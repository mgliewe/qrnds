
#include <math.h>
#include <stdlib.h>

#include "qrnd-core.h"
#include "gaussian.h"

namespace QRND {

GaussianProducer::GaussianProducer(const char *name, int buffersize, int num_buffer)
    : Filter(name, 1),
      buffersize(buffersize), num_buffer(num_buffer),
      mean(127), stddev(100), hasSpare(false),
      entropy_frame(0)
{
    allocate_buffers(buffersize, num_buffer);
}

GaussianProducer::~GaussianProducer() {
    release_buffers();  
}


double GaussianProducer::get_entropy() {
    double v;
    while (!entropy_frame) {
        entropy_frame = receive();
        entropy_p = 0;
    } 
    while (entropy_p>=entropy_frame->count) {
        entropy_frame->give_back();
        entropy_frame = receive();
        entropy_p = 0;
    }    
    v = entropy_frame->data[entropy_p++];
    return v / 256;
}

void GaussianProducer::run() {
    Frame *output;

    double spare;
    bool hasSpare;

    output = get_frame();
    for (;;) {
        value_type v = generateGaussian();
        if (v>255) v = 255;
        output->data[output->count++] = v;
        if (output->count>=output->capacity) {
            send(0, output);
            output = get_frame();
        }
    }
}

double GaussianProducer::generateGaussian() {
    if (hasSpare) {
        hasSpare = false;
        return spare * stddev + mean;
    } else {
        double u, v, s;
        do {
            u = get_entropy() * 2.0 - 1.0;
            v = get_entropy() * 2.0 - 1.0;
            s = u * u + v * v;
        } while (s >= 1.0 || s == 0.0);
        s = sqrt(-2.0 * log(s) / s);
        spare = v * s;
        hasSpare = true;
        return mean + stddev * u * s;
    }
}

void GaussianProducer::set_mean(int v) {
    mean = v;
}

void GaussianProducer::set_derivation(int v) {
    stddev = v;
}

} // end namespace
