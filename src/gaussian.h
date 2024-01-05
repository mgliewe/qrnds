#include "qrnd-core.h"

namespace QRND {

class GaussianProducer : public Filter, public Runnable {
protected:
    int buffersize;
    int num_buffer;

    double mean;
    double stddev;

    double spare;
    bool hasSpare;

    Frame *entropy_frame;
    int entropy_p;

    double get_entropy();
    double generateGaussian();

public:
    GaussianProducer(const char *name, int buffersize = 32*K, int num_buffer=8);
    ~GaussianProducer();

    void set_derivation(int v);
    void set_mean(int v);
    int get_mean();
    int get_derivation();

protected:
    void run();
};

} // end namespace
