

#include "fauchinger.h"

#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


/**
 * randomness extraction function, as described in the paper
 * "True randomness from realistic quantum devices", Frauchiger et.al.
 * 
 * @param N     the number of input bits
 * @param in    an input array of N bits stores as N/64 64−bit integers
 * @param L     the number of output bits
 * @param out   an output array of L bits stored as L/64 64− bit integers
 * @param m     a random matrix of l∗n bits, stored in L∗N/64 64−bit integers
 */
static void extract(const unsigned N, uint64_t const *in, 
                    const unsigned L, uint64_t *out, 
                    uint64_t const *m) {

    assert ( N%64==0 && L%64==0 ) ;

    // ???? what does that constant really mean?
    const uint64_t CONSTANT_MASK = 0x1111111111111111UL;
    // ???? where does that constant come from?
    const unsigned CONSTANT_RSHIFT = 60; 

    int ind = 0;
    // perform a matrix−vector multiplication by looping over all rows
    // the outer loop over all words
    for (int i = 0 ; i < L/64; ++i) {
        out[i] = 0;
        // the inner loop over all bits in the word
        for (unsigned j = 0 ; j < 64; ++j ) {
            uint64_t parity = m[ind++] & in[0];
            // perform a vector − vector multiplication using bit operations
            for (unsigned k = 1 ; k < N / 64; ++k )
                parity ^= m[ind++] & in[k];

            // finally obtain the bit parity
            parity ^= parity >> 1;
            parity ^= parity >> 2;
            parity = ( parity & CONSTANT_MASK ) * CONSTANT_MASK;

            // and set the j−th output bit of the i−th output word
            out[i] |= ((parity >> CONSTANT_RSHIFT) & 1) << j;       
        }
    }
}

Fauchinger::Fauchinger(const char *name, int input_bits, int output_bits, const char *m_matrix_file)
    : QRND::Filter(name),
      N(input_bits), L(output_bits)
{
    read_m_matrix(m_matrix_file);
}

void Fauchinger::read_m_matrix(const char *filename) {
    int sz = N * L / 64;

    m = new uint64_t[sz / 8];

    int fd = open(filename, O_RDONLY);
    if (fd<0) {
        throw "cannot open seed file for reading";
    }
    uint8_t *p = (uint8_t *) m;
    while (sz>0) {
        int n = read(fd, p, sz);
        if (n<0) {
            throw "short read on  seed file";
        }
        sz -= n;
    }    
}

void Fauchinger::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;

        QRND::Frame *in_buffer = receive();
        QRND::Frame *out_buffer = get_frame();

        assert(in_buffer->count <= N/64);
        assert(out_buffer->count <= L/64);



    }
}

