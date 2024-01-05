
#include "qrnd-measurement.h"
#include <cassert>

namespace QRND {


FrameMeter::FrameMeter(const char *name, int buffersize, int num_buffer) 
    : Filter(name, 3),
      buffersize(buffersize), num_buffer(num_buffer),
      windowsize(8*K),
      rms_counter(0), avg_counter(0), value_counter(0)
{
    allocate_buffers(buffersize, num_buffer);
}

FrameMeter::~FrameMeter() {
    release_buffers();
}

unsigned long FrameMeter::get_rms() {
    if (value_counter==0) return 0;
    return sqrt(rms_counter / value_counter);
}

unsigned long FrameMeter::get_avg() {
    if (value_counter==0) return 0;
    return avg_counter / value_counter;
}

void FrameMeter::set_windowsize(int sz) {
    windowsize = sz;
}

void FrameMeter::run() {
    rms_counter = avg_counter = 0;
    value_counter=0;

    Frame *rms_out = get_frame();
    assert( rms_out!=0 );
    Frame *avg_out = get_frame();
    assert( avg_out != 0);

    for (;;) {

        Frame *f = receive();
        for (int n=0; n<f->count; ++n) {
            value_type v = f->data[n];
            rms_counter += v*v;
            avg_counter += v;
            value_counter++;

            if (value_counter>=windowsize) {
                value_type rms = get_rms();
                value_type avg = get_avg();

                rms_out->data[rms_out->count++] = rms;
                avg_out->data[avg_out->count++] = avg;

                rms_counter = avg_counter = 0;
                value_counter=0;

                if (rms_out->count >= rms_out->capacity) {
                    send(1, rms_out);
                    rms_out = get_frame();
                    assert( rms_out!=0 );

                    send(2, avg_out);
                    avg_out = get_frame();
                    assert( avg_out!=0 );

                }

            }
        }
        send(0, f);
    }
}

FrameCounter::FrameCounter(const char *name, int buffersize, int num_buffer)
    : Filter(name, 3),
      buffersize(buffersize), num_buffer(num_buffer), windowsize(1 * K)
{
    allocate_buffers(buffersize, num_buffer);
}

FrameCounter::~FrameCounter() {
    release_buffers();
}

void FrameCounter::set_windowsize(int sz) {
    windowsize = sz;
}

void FrameCounter::run() {

    counter = 0;
    timestamp = get_time();
    Frame *out_bw = get_frame();
    Frame *out_e = get_frame();
    for (;;) {
        Frame *f = receive();
        if (f) {
            counter += f->count;
            while (counter>=windowsize) {
                int bw = get_bandwidth();
                timestamp = get_time();
                counter = 0;

                int e = 0;
                while(bw>100) {
                    bw /= 10; e++;
                }
                out_bw->data[out_bw->count++] = bw;
                out_e->data[out_e->count++] = e;
                if (out_bw->count>=out_bw->capacity) {
                    send(1, out_bw);
                    send(2, out_e);
                    out_bw = get_frame();
                    out_e = get_frame();
                }
            }
            send(0, f);
        }
    }
}

unsigned long FrameCounter::get_bandwidth() {
    unsigned long tm = get_time();
    tm -= timestamp;
    if (tm) return (unsigned long) counter * 1000 / tm;
    else return 0;
}


Histogram::Histogram(const char *name, int num_buffer)
    : Filter(name, 2), num_buffer(num_buffer)
{
    allocate_buffers(256, num_buffer);
}

Histogram::~Histogram() {
    release_buffers();
}

void Histogram::set_windowsize(int sz) {
    windowsize = sz;
}

void Histogram::run() {

    Frame *histogram;
    histogram = get_frame();
    histogram->count = 256;
    for (int n=0; n<256; ++n)
        histogram->data[n] = 0;
    int counter = 0;
    for (;;) {
        Frame *input = receive();
        if (input) {
            for (int n=0; n<input->count; ++n) {
                int value = input->data[n] & 255;
                if (histogram->data[value]<255)
                    histogram->data[value] ++;
                counter++;
                if (counter>=windowsize) {
                    send(1, histogram);
                    histogram = get_frame();
                    histogram->count = 256;
                    for (int n=0; n<256; ++n)
                        histogram->data[n] = 0;
                    counter = 0;
                }
            }
        }
        send(0, input);
    }
}


Derivation::Derivation(const char *name, int buffersize, int num_buffer) 
    : Filter(name, 3),
      buffersize(buffersize), num_buffer(num_buffer),
      windowsize(8*K),
      mean_counter(0), derivation_counter(0), value_counter(0)
{
    allocate_buffers(buffersize, num_buffer);
}

Derivation::~Derivation() {
    release_buffers();
}

int Derivation::get_derivation() {
    if (value_counter==0) return 0;
    return sqrt(derivation_counter / value_counter);
}

int Derivation::get_mean() {
    if (value_counter==0) return 0;
    return mean_counter / value_counter;
}

void Derivation::set_windowsize(int sz) {
    windowsize = sz;
}

void Derivation::run() {
    mean_counter = derivation_counter = 0;
    value_counter=0;

    Frame *mean_out = get_frame();
    assert( mean_out!=0 );
    Frame *derivation_out = get_frame();
    assert( derivation_out != 0);

    // M = sum(x)/n
    // D = sqrt( sum(sqr(x-M)) / n )
    for (;;) {

        Frame *f = receive();
        for (int n=0; n<f->count; ++n) {
            value_type v = f->data[n];

            value_counter += 1;
            mean_counter += v;
            value_type m = mean_counter / value_counter;
            derivation_counter += (v-m)*(v-m);

            if (value_counter>=windowsize) {
                value_type mean = get_mean();
                value_type derivation = get_derivation();

                mean_out->data[mean_out->count++] = mean;
                derivation_out->data[derivation_out->count++] = derivation;

                if (mean_out->count >= mean_out->capacity) {
                    send(1, mean_out);
                    mean_out = get_frame();
                    assert( mean_out!=0 );

                    send(2, derivation_out);
                    derivation_out = get_frame();
                    assert( derivation_out!=0 );

                    value_counter=0;
                    mean_counter = derivation_counter = 0;
                }

            }
        }
        send(0, f);
    }
}


} // end namespace