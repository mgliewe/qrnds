
#ifndef __FAUCHINGER_H__
#define __FAUCHINGER_H__

#include "qrnd-core.h"

class Fauchinger : public Consumer, public Producer, public Runnable {
private:
    unsigned N, L;
    uint64_t *m;
public:
    Fauchinger(const char *name, int input_bits = 1024, int output_bits = 768, const char *m_matrix_file = "m_matrix.bin");
    Fauchinger(std::string name, int input_bits = 1024, int output_bits = 768, const char *m_matrix_file = "m_matrix.bin")
        : Fauchinger(name.c_str(), input_bits, output_bits) { }

protected:
    void run();

    void read_m_matrix(const char *filename);
};

#endif // __FAUCHINGER_H__
