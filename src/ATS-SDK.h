#ifndef __ATS_SDK_H__
#define __ATS_SDK_H__

#include "../include/AlazarApi.h"

#include <stdint.h>
#include <string>

namespace ATS {

struct Configuration {
    int system_id;          ///< system id
    int board_id;           ///< board id
    int channel;            ///< input channel to capture
    long samplerate;        ///< samplerate for capture
    int input_quadrants;    ///< input voltage type, 
                            ///< use 0: -input_range < v < input_range, 
                            ///<    -1: -input_range < v < 0
                            ///<     1: 0 < v < input_range  
    int input_range;        ///< input voltage sensivity in mV
    bool eigth_bit;         
    bool pack_12_bit;
    unsigned record_size;

    Configuration() : 
        system_id(0), board_id(0),
        channel(1),
        samplerate(1800*1000*1000),
        input_quadrants(0), input_range(1250),
        eigth_bit(false), pack_12_bit(false), 
        record_size(8 * 1024)
        { }

    Configuration(const char *config) : 
        Configuration()
        { parse_config_string(config); }

    void parse_config_string(const char *config);
    void set_config(const char *config, const char *value);
    void set_config(const char *config, unsigned value);
};


class Board {
    HANDLE handle;
public:
    int system_id, board_id;
    long onboard_memory;
    int bits_per_sample;

    Board(int system_id, int board_id);

    int board_kind();
    const char *board_name();
    const char *board_revision();
    const char *cpld_version();

    uint32_t query_capability(ALAZAR_CAPABILITIES cap);


    void set_record_size(unsigned pre_trigger_samples, unsigned post_trigger_samples);
    void set_capture_clock(ALAZAR_CLOCK_SOURCES source,  ALAZAR_SAMPLE_RATES samplerate, ALAZAR_CLOCK_EDGES edge, int decimation);
    void input_control(ALAZAR_CHANNELS channel, ALAZAR_COUPLINGS coupling, ALAZAR_INPUT_RANGES input_range, ALAZAR_IMPEDANCES impedance);

    void set_trigger_operation(
        ALAZAR_TRIGGER_OPERATIONS operation,
        ALAZAR_TRIGGER_ENGINES engine1,
        ALAZAR_TRIGGER_SOURCES source1,
        ALAZAR_TRIGGER_SLOPES slope1,
        unsigned level1,
        ALAZAR_TRIGGER_ENGINES engine2,
        ALAZAR_TRIGGER_SOURCES source2,
        ALAZAR_TRIGGER_SLOPES slope2,
        unsigned level2
    );

    void set_trigger_timeout(unsigned ticks);
    void set_trigger_delay(unsigned ticks);

    void get_channel_info(int64_t &msize, uint8_t &bits);

    long get_parameter(ALAZAR_CHANNELS channel, ALAZAR_PARAMETERS parameter);
    void set_parameter(ALAZAR_CHANNELS channel, ALAZAR_PARAMETERS parameter, long value);

    uint16_t *alloc_buffer(size_t sz) {
        return (uint16_t *) AlazarAllocBufferU16Ex(handle, sz);
    }

    void free_buffer(uint16_t *buffer) {
        AlazarFreeBufferU16Ex(handle, buffer);
    }

    void before_async_read(
        ALAZAR_CHANNELS channelSelect, 
        long transferOffset, U32 transferLength, 
        U32 recordsPerBuffer, U32 recordsPerAcquisition, 
        unsigned flags);
    void start_capture();
    void abort_async_read();

    void post_async_buffer(void *buffer, unsigned buffer_size);
    RETURN_CODE wait_async_buffer_complete(void *buffer, unsigned timeout);


};



const char * driver_version();
const char * sdk_version();
int num_boards();
int num_systems();
int num_boards(int system_id);

extern const struct sample_rates {
    long rate;
    ALAZAR_SAMPLE_RATES code;
} sample_rates[];

ALAZAR_SAMPLE_RATES to_samplerate_code(long rate);

extern const struct input_ranges {
    int q;
    int mv;
    ALAZAR_INPUT_RANGES code;
} input_ranges[];

ALAZAR_INPUT_RANGES to_inputrange_code(int quadrants, int milli_volts);

} /* end namespace */





#endif
