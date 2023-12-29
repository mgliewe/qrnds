#include <math.h>
#include <stdlib.h>


class GaussianProducer {
private:
    double mean;
    double stddev;

    double spare;
    bool hasSpare;

protected:
    GaussianProducer();

    double get_entropy();

    double generateGaussian();
};

double GaussianProducer::get_entropy() {
    return (rand() / ((double)RAND_MAX));
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

