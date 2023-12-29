
#include "qrnd-measurement.h"


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

}

int FrameMeter::get_rms() {
    if (value_counter==0) return 0;
    return sqrt(rms_counter / value_counter);
}

int FrameMeter::get_avg() {
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
    Frame *avg_out = get_frame();

    for (;;) {

        Frame *f = receive();
        for (int n=0; n<f->count; ++n) {
            value_type v = f->data[n];
            rms_counter += v*v;
            avg_counter += v;
            value_counter++;

            if (value_counter==windowsize) {
                value_type rms = get_rms();

                rms_out->data[rms_out->count++] = rms;
                if (rms_out->count = rms_out->capacity) {
                    send(1, rms_out);
                }
                rms_out = get_frame();

                value_type avg = get_avg();
                avg_out->data[avg_out->count++] = avg;
                if (avg_out->count = avg_out->capacity) {
                    send(2, avg_out);
                }
                avg_out = get_frame();
            }
        }
        f->give_back();

    }
}

} // end namespace