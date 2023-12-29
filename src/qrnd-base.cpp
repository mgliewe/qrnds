#include "qrnd-base.h"

#include <iostream>

namespace QRND {



NullProducer::NullProducer(const char *name, int buffersize, int num_buffer) 
  : Producer(name, 1), buffersize(buffersize), num_buffer(num_buffer)
{
    allocate_buffers(buffersize, num_buffer);
}

NullProducer::~NullProducer() { 
    release_buffers();
}

void NullProducer::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;
        Frame *b = get_frame();
        send(0, b);
    }
}


RandProducer::RandProducer(const char *name, int buffersize, int num_buffer) 
  : Producer(name, 1), buffersize(buffersize), num_buffer(num_buffer)
{
    allocate_buffers(buffersize, num_buffer);
    seed = time(NULL);
}

RandProducer::~RandProducer() {
    release_buffers();
}

void RandProducer::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;
        Frame *b = get_frame();
        for (int n=0; n<b->capacity; ++n) {
            b->data[n] = rand_r(&seed);
        }
        b->count = b->capacity;
        send(0, b);
    }
}

void RandProducer::set_seed(int new_seed) {
    this->seed = new_seed; 
}


VoidSink::VoidSink(const char *name)
    : Consumer(name)
{
}

VoidSink::~VoidSink() {
}

void VoidSink::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }

        Frame *f = receive();
        f->give_back();

        if (is_stopped())
            break;
    }
}



FrameBuffer::FrameBuffer(const char *name)
    : Consumer(name), last_frame(nullptr)
{
}

FrameBuffer::~FrameBuffer() {
}

void FrameBuffer::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }

        Frame *new_frame = receive();
        if (last_frame)
            last_frame->give_back();
        last_frame = new_frame;

        if (is_stopped())
            break;
    }
}




LazyForward::LazyForward(const char *name, int buffersize, int num_buffer)
    : Filter(name, 1), buffersize(buffersize), num_buffer(num_buffer)
{
    allocate_buffers(buffersize, num_buffer);
}

LazyForward::~LazyForward() {
    release_buffers();
}

void LazyForward::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }

        Frame *in = receive();
        Frame *out = get_frame_if_available();
        if (out) {
            if (out->capacity < in->count) {
                QRND::abort("LazyForward: input buffer to large");
            }
            for (int n=0; n<in->count; ++n) {
                out->data[n] = in->data[n];
            }
            out->count = in->count;
            send(0, out);
        }
        in->give_back();

        if (is_stopped())
            break;
    }
}


StreamSplitter::StreamSplitter(const char *name, int buffersize, int num_buffer, int num_outputs)
    : Filter(name, num_outputs), buffersize(buffersize), num_buffer(num_buffer)
{
    //TODO:
}

StreamSplitter::~StreamSplitter() {
    release_buffers();
}

void StreamSplitter::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }

        Frame *in = receive();

        for (int n=0; n<this->num_outputs; ++n) {
            if (this->output_slots[n]) {
                Frame *out = get_frame();
                if (out->capacity < in->count) {
                    QRND::abort("StreamSplitter: input buffer to large");
                }
                for (int n=0; n<in->count; ++n) {
                    out->data[n] = in->data[n];
                }
                out->count = in->count;
                send(0, out);
            }
        }
        in->give_back();

        if (is_stopped())
            break;
    }
}

StreamBalancer::StreamBalancer(const char *name, int num_outputs)
    : Consumer(name), Producer(name, num_outputs), next_output(0) 
{ 
}

StreamBalancer::~StreamBalancer() {
}

void StreamBalancer::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;

        Frame *frame = receive();
        
        // find next output and emit 
        int n = next_output;
        do {
            // if slot is connected, emit
            if (output_slots[n]) {
                send(n, frame);
                break;
            } 
            // advance to next slot
            n++; 
            if (n>num_outputs) {
                n = 0;
            }
            // if we reached next_output, there are no connections; just dissmiss frame
            if (n==next_output) {
                frame->give_back();
            }
        } while (n!=next_output);

        // advance to next slot for next iteration
        n = n + 1;
        if (n>num_outputs) {
            n = 0;
        }
        next_output = n;
    }

}



} // end namespace