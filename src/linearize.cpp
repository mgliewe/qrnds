



#include "linearize.h"

#include <string.h>


void Linearize::run() {

    bool calibrating = true;
    int frame_counter = 0;
    int value_counter = 0;

    unsigned *histogram = (unsigned *) calloc(sizeof(unsigned), MAX_VALUE*2);
    assert( histogram != 0 );

    unsigned *dat_counter = (unsigned *) calloc(sizeof(unsigned), MAX_VALUE*2);
    assert( dat_counter != 0 );

    unsigned *dat_propabillity = (unsigned *) calloc(sizeof(unsigned), MAX_VALUE*2);
    assert( dat_propabillity != 0 );

    Frame *output = get_frame();

    set_fault_condition(true);

    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;

        Frame *input = receive();

        for (int n=0; n<input->count; ++n) {
            // get next value and scale to positive number
            int n = input->data[m];
            n += MAX_VALUE;
            assert( n>=0 );
            assert( n<MAX_VALUE*2 );

            // update histogram
            histogram[n] ++;

            // if we have valid propabillities
            if (!calibrating) {
                // mark occurence of datum, and pass along if propabillity is reached
                dat_counter[n]++;
                if (dat_counter[n] > dat_propabillity[n]) {
                    dat_counter[n] = 0;
                    output->data[output->count++] = n;
                    // if output frame is filled, emit frame
                    if (output->count==output->capacity) {
                        send(0, output);
                        output = get_frame();
                    }
                }
            }
        }

        frame_counter++;
        if (frame_counter>=calibration_frames) {
            calibrating = update_propabillities(histogram, value_counter, dat_propabillity);
            set_fault_condition(calibrating);
            
            frame_counter = 0;
            value_counter = 0;
            bzero(histogram, sizeof(unsigned) * MAX_VALUe * 2);
            bzero(dat_counter, sizeof(unsigned) * MAX_VALUe * 2);
        }

    }

}