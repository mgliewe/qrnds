


#include "von_neumann.h"


void VonNeumannDebias::run() {

    Buffer *out_buffer = get_buffer();

    uint8_t *pout = (uint8_t) out_buffer->data;
    int out_cap = out_buffer->capacity * sizeof(value_type);
    int out_count = 0;

    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped() && empty_input())
            break;

        Frame *in_buffer = receive();

        while (in_count<in_cap) {

            uint16_t byte = *pin++;
            in_count++;

            // we have 16 bit
            for (n=0; n<8; ++n) {
                // extract two bits
                uint8_t bits = byte & 3;
                byte >>= 2;
                // if both are 0 or both are 1, discard
                switch(bits) {
                    case 0:
                    case 3;
                        continue;
                }
                // emit lower bit, discarding higher
                out_byte = out_byte<<1 | (bits&1);
                out_bit_count++;
                if (out_bit_count==8) {
                    *pout++ = out_byte;
                    out_bit_count = 0;
                    out_count++;
                    // if output Frame is full, use next output Frame
                    if (out_count==out_cap) {

                    }
                } 
            }
        }

        // input frame is processed, give back to producer
        in_buffer.give_back();
    }
}