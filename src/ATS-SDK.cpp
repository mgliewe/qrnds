

#include <stdio.h>
#include "ATS-SDK.h"
#include <cstring>
#include <stdexcept>
namespace ATS {



#define ATS_Exception(code) \
    std::runtime_error( __FILE__ ":" + std::to_string(__LINE__) + ": "  \
        + std::string(AlazarErrorToText(code)))


const char * driver_version() {
    static char version_string[32];
    uint8_t major, minor, revision;
    AlazarGetDriverVersion(&major, &minor, &revision);

    sprintf(version_string, "%d.%d.%d", major, minor, revision);
    return version_string;
}


const char * sdk_version() {
    static char version_string[32];
    uint8_t major, minor, revision;
    AlazarGetSDKVersion(&major, &minor, &revision);

    sprintf(version_string, "%d.%d.%d", major, minor, revision);
    return version_string;
}


int num_systems() {
    return AlazarNumOfSystems();
}

int num_boards() {
    return AlazarBoardsFound();
}

int num_boards(int system_id) {
    return AlazarBoardsInSystemBySystemID(system_id);
}

# define KS 1000L
# define MS KS*KS

const struct sample_rates sample_rates[] = {
    {    1*KS, SAMPLE_RATE_1KSPS },
    {    2*KS, SAMPLE_RATE_2KSPS },
    {    5*KS, SAMPLE_RATE_5KSPS },
    {   10*KS, SAMPLE_RATE_10KSPS },
    {   20*KS, SAMPLE_RATE_20KSPS },
    {   50*KS, SAMPLE_RATE_50KSPS },
    {  100*KS, SAMPLE_RATE_100KSPS },
    {  200*KS, SAMPLE_RATE_200KSPS },
    {  500*KS, SAMPLE_RATE_500KSPS },
    {    1*MS, SAMPLE_RATE_1MSPS },
    {    2*MS, SAMPLE_RATE_2MSPS },
    {    5*MS, SAMPLE_RATE_5MSPS },
    {   10*MS, SAMPLE_RATE_10MSPS },
    {   20*MS, SAMPLE_RATE_20MSPS },
    {   25*MS, SAMPLE_RATE_25MSPS },
    {   50*MS, SAMPLE_RATE_50MSPS },
    {  100*MS, SAMPLE_RATE_100MSPS },
    {  125*MS, SAMPLE_RATE_125MSPS },
    {  160*MS, SAMPLE_RATE_160MSPS },
    {  180*MS, SAMPLE_RATE_180MSPS },
    {  200*MS, SAMPLE_RATE_200MSPS },
    {  250*MS, SAMPLE_RATE_250MSPS },
    {  300*MS, SAMPLE_RATE_300MSPS },
    {  350*MS, SAMPLE_RATE_350MSPS },
    {  370*MS, SAMPLE_RATE_370MSPS },
    {  400*MS, SAMPLE_RATE_400MSPS },
    {  500*MS, SAMPLE_RATE_500MSPS },
    {  800*MS, SAMPLE_RATE_800MSPS },
    { 1000*MS, SAMPLE_RATE_1000MSPS },
    { 1200*MS, SAMPLE_RATE_1200MSPS },
    { 1500*MS, SAMPLE_RATE_1500MSPS },
    { 1600*MS, SAMPLE_RATE_1600MSPS },
    { 1800*MS, SAMPLE_RATE_1800MSPS },
    { 2000*MS, SAMPLE_RATE_2000MSPS },
    { 2400*MS, SAMPLE_RATE_2400MSPS },
    { 3000*MS, SAMPLE_RATE_3000MSPS },
    { 3600*MS, SAMPLE_RATE_3600MSPS },
    { 4000*MS, SAMPLE_RATE_4000MSPS },
    {  0, (ALAZAR_SAMPLE_RATES) 0 }
};

ALAZAR_SAMPLE_RATES to_samplerate_code(long rate) {
    for (const struct sample_rates *p = sample_rates; p->rate; ++p) {
        if (p->rate==rate)
            return p->code;
    }
    throw std::runtime_error("unsupported sampling rate: " + std::to_string(rate));
}

const struct input_ranges input_ranges[] = {
    { -1,     0, INPUT_RANGE_HIFI },               ///< no gain
    {  0,     0, INPUT_RANGE_HIFI },               ///< no gain
    { +1,     0, INPUT_RANGE_HIFI },               ///< no gain

    {  0,    20, INPUT_RANGE_PM_20_MV },           ///< +/- 20mV
    {  0,    40, INPUT_RANGE_PM_40_MV },           ///< +/- 40mV
    {  0,    50, INPUT_RANGE_PM_50_MV },           ///< +/- 50mV
    {  0,    80, INPUT_RANGE_PM_80_MV },           ///< +/- 80mV
    {  0,   100, INPUT_RANGE_PM_100_MV },          ///< +/- 100mV
    {  0,   125, INPUT_RANGE_PM_125_MV },          ///< +/- 125mV
    {  0,   200, INPUT_RANGE_PM_200_MV },          ///< +/- 200mV
    {  0,   250, INPUT_RANGE_PM_250_MV },          ///< +/- 250mV
    {  0,   400, INPUT_RANGE_PM_400_MV },          ///< +/- 400mV
    {  0,   500, INPUT_RANGE_PM_500_MV },          ///< +/- 500mV
    {  0,   800, INPUT_RANGE_PM_800_MV },          ///< +/- 800mV
    {  0,  1000, INPUT_RANGE_PM_1_V },             ///< +/- 1V
    {  0,  1250, INPUT_RANGE_PM_1_V_25 },          ///< +/- 1.25V
    {  0,  2000, INPUT_RANGE_PM_2_V },             ///< +/- 2V
    {  0,  2500, INPUT_RANGE_PM_2_V_5 },           ///< +/- 2.5V
    {  0,  4000, INPUT_RANGE_PM_4_V },             ///< +/- 4V
    {  0,  5000, INPUT_RANGE_PM_5_V },             ///< +/- 5V
    {  0,  8000, INPUT_RANGE_PM_8_V },             ///< +/- 8V
    {  0, 10000, INPUT_RANGE_PM_10_V },            ///< +/- 10V
    {  0, 16000, INPUT_RANGE_PM_16_V },            ///< +/- 16V
    {  0, 20000, INPUT_RANGE_PM_20_V },            ///< +/- 20V
    {  0, 40000, INPUT_RANGE_PM_40_V },            ///< +/- 40V

    { +1,    40, INPUT_RANGE_0_TO_40_MV },         ///< 0 to 40mV
    { +1,    80, INPUT_RANGE_0_TO_80_MV },         ///< 0 to 80mV
    { +1,   100, INPUT_RANGE_0_TO_100_MV },        ///< 0 to 100mV
    { +1,   100, INPUT_RANGE_0_TO_160_MV },        ///< 0 to 160mV
    { +1,   200, INPUT_RANGE_0_TO_200_MV },        ///< 0 to 200mV
    { +1,   250, INPUT_RANGE_0_TO_250_MV },        ///< 0 to 250mV
    { +1,   400, INPUT_RANGE_0_TO_400_MV },        ///< 0 to 400mV
    { +1,   500, INPUT_RANGE_0_TO_500_MV },        ///< 0 to 500mV
    { +1,   800, INPUT_RANGE_0_TO_800_MV },        ///< 0 to 800mV
    { +1,  1000, INPUT_RANGE_0_TO_1_V },           ///< 0 to 1V
    { +1,  1600, INPUT_RANGE_0_TO_1600_MV },       ///< 0 to 1.6V
    { +1,  2000, INPUT_RANGE_0_TO_2_V },           ///< 0 to 2V
    { +1,  2500, INPUT_RANGE_0_TO_2_V_5 },         ///< 0 to 2.5V
    { +1,  4000, INPUT_RANGE_0_TO_4_V },           ///< 0 to 4V
    { +1,  5000, INPUT_RANGE_0_TO_5_V },           ///< 0 to 5V
    { +1,  8000, INPUT_RANGE_0_TO_8_V },           ///< 0 to 8V
    { +1, 10000, INPUT_RANGE_0_TO_10_V },          ///< 0 to 10V
    { +1, 16000, INPUT_RANGE_0_TO_16_V },          ///< 0 to 16V
    { +1, 20000, INPUT_RANGE_0_TO_20_V },          ///< 0 to 20V
    { +1, 32000, INPUT_RANGE_0_TO_32_V },          ///< 0 to 32V
    { +1, 80000, INPUT_RANGE_0_TO_80_V },          ///< 0 to 80V

    { -1,    40, INPUT_RANGE_0_TO_MINUS_40_MV },   ///< 0 to -40mV
    { -1,    80, INPUT_RANGE_0_TO_MINUS_80_MV },   ///< 0 to -80mV
    { -1,   100, INPUT_RANGE_0_TO_MINUS_100_MV },  ///< 0 to -100mV
    { -1,   160, INPUT_RANGE_0_TO_MINUS_160_MV },  ///< 0 to -160mV
    { -1,   200, INPUT_RANGE_0_TO_MINUS_200_MV },  ///< 0 to -200mV
    { -1,   250, INPUT_RANGE_0_TO_MINUS_250_MV },  ///< 0 to -250mV
    { -1,   400, INPUT_RANGE_0_TO_MINUS_400_MV },  ///< 0 to -400mV
    { -1,   500, INPUT_RANGE_0_TO_MINUS_500_MV },  ///< 0 to -500mV
    { -1,   800, INPUT_RANGE_0_TO_MINUS_800_MV },  ///< 0 to -800mV
    { -1,  1000, INPUT_RANGE_0_TO_MINUS_1_V },     ///< 0 to -1V
    { -1,  1600, INPUT_RANGE_0_TO_MINUS_1600_MV }, ///< 0 to -1.6V
    { -1,  2000, INPUT_RANGE_0_TO_MINUS_2_V },     ///< 0 to -2V
    { -1,  2500, INPUT_RANGE_0_TO_MINUS_2_V_5 },   ///< 0 to -2.5V
    { -1,  4000, INPUT_RANGE_0_TO_MINUS_4_V },     ///< 0 to -4V
    { -1,  5000, INPUT_RANGE_0_TO_MINUS_5_V },     ///< 0 to -5V
    { -1,  8000, INPUT_RANGE_0_TO_MINUS_8_V },     ///< 0 to -8V
    { -1, 10000, INPUT_RANGE_0_TO_MINUS_10_V },    ///< 0 to -10V
    { -1, 16000, INPUT_RANGE_0_TO_MINUS_16_V },    ///< 0 to -16V
    { -1, 20000, INPUT_RANGE_0_TO_MINUS_20_V },    ///< 0 to -20V
    { -1, 32000, INPUT_RANGE_0_TO_MINUS_32_V },    ///< 0 to -32V
    { -1, 80000, INPUT_RANGE_0_TO_MINUS_80_V },    ///< 0 to -80V

    { 0,      0, (ALAZAR_INPUT_RANGES) 0 }

};

ALAZAR_INPUT_RANGES to_inputrange_code(int q, int mv) {
    for (const struct input_ranges *p = input_ranges; p->code; ++p) {
        if (p->q == q && p->mv==mv)
            return p->code;
    }
    throw std::runtime_error("unsupported input range: " + std::to_string(q) + "," + std::to_string(mv));
}


void Configuration::set_config(const char *config, const char *value) {

    if (strcmp(config, "device")==0) {

    } else if (strcmp(config, "channel")==0) {

    } else if (strcmp(config, "rate")==0) {
        
    } else if (strcmp(config, "range")==0) {

    } else if (strcmp(config, "pack")==0) {

    } else if (strcmp(config, "eightbit")==0) {

    } else if (strcmp(config, "recordsize")==0) {

    }
}

void Configuration::parse_config_string(const char *config) {

    char *cfg_copy = strdup(config);
    char *cfg = cfg_copy;
    char *p = cfg;
    for (;*p;) {
        char *ident, *value, *p;
        while (isblank(*cfg)) cfg++;
        p=ident=cfg;
        while(isalnum(*p)) 
            *p++;
        char c=*p; *p++ = 0;
        while (isblank(c))
            c=*p++;
        if (c=='=') {
            while(isblank(*p))
                p++;
            p = value = cfg;
            while (*p && *p!=',') {
                p++;
            }
            *p = 0;
            set_config(ident, value);
        } else {
            set_config(ident, "");
        }

        while(*p==',') p++;
    }
    free(cfg_copy);
}

Board::Board(int system_id, int board_id) {
    handle = AlazarGetBoardBySystemID(system_id, board_id);
    if (handle==NULL) {
        throw std::runtime_error("Alazar Capture board was not found at ["
                                  + std::to_string(system_id) + ":" + std::to_string(board_id) 
                                  + "]");
    }

    uint32_t mem;
    uint8_t bits;
    AlazarGetChannelInfo(handle, &mem, &bits);
    onboard_memory = mem;
    bits_per_sample = bits;

    
}

const char * Board::cpld_version() {
    static char version_string[32];
    uint8_t major, minor, revision = 0;
    AlazarGetCPLDVersion(handle, &major, &minor);

    sprintf(version_string, "%d.%d.%d", major, minor, revision);
    return version_string;
}

int Board::board_kind() {
    return AlazarGetBoardKind(handle);
}

const char * const board_names[] = {
    "ATS_NONE",
    "ATS850",   "ATS310",   "ATS330",   "ATS855",
    "ATS315",   "ATS335",   "ATS460",   "ATS860",
    "ATS660",   "ATS665",   "ATS9462",  "ATS9434",
    "ATS9870",  "ATS9350",  "ATS9325",  "ATS9440",
    "ATS9410",  "ATS9351",  "ATS9310",  "ATS9461",
    "ATS9850",  "ATS9625",  "ATG6500",  "ATS9626",
    "ATS9360",  "AXI8870",  "ATS9370",  "ATU7825",
    "ATS9373",  "ATS9416",  "ATS9637",  "ATS9120",
    "ATS9371",  "ATS9130",  "ATS9352",  "ATS9453",
    "unknown"
};

const char *Board::board_name() {
    int kind = AlazarGetBoardKind(handle);
    if (kind>ATS_LAST) kind = ATS_LAST;
    return board_names[kind];
}

const char * Board::board_revision() {
    static char version_string[32];
    uint8_t major, minor, revision = 0;
    AlazarGetBoardRevision(handle, &major, &minor);

    sprintf(version_string, "%d.%d.%d", major, minor, revision);
    return version_string;
}


unsigned int Board::query_capability(ALAZAR_CAPABILITIES cap) {
    uint32_t out;
    AlazarQueryCapability(handle, cap, 0, &out);
    return out;
}


void Board::set_trigger_operation(
    ALAZAR_TRIGGER_OPERATIONS operation,
    ALAZAR_TRIGGER_ENGINES engine1,
    ALAZAR_TRIGGER_SOURCES source1,
    ALAZAR_TRIGGER_SLOPES slope1,
    unsigned level1,
    ALAZAR_TRIGGER_ENGINES engine2,
    ALAZAR_TRIGGER_SOURCES source2,
    ALAZAR_TRIGGER_SLOPES slope2,
    unsigned level2
) {
    RETURN_CODE ret = AlazarSetTriggerOperation(
        handle, operation, 
        engine1, source1, slope1, level1,
        engine2, source2, slope2, level2
    );

    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

void Board::set_trigger_timeout(unsigned ticks) {
    RETURN_CODE ret = AlazarSetTriggerTimeOut(handle, ticks);

    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

void Board::set_trigger_delay(unsigned ticks) {
    RETURN_CODE ret = AlazarSetTriggerDelay(handle, ticks);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

void Board::get_channel_info(int64_t &msize, uint8_t &bits) {
    S64 m;
    RETURN_CODE ret = AlazarGetChannelInfoEx(handle, &m, &bits);
    msize = m;
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

void Board::set_parameter(ALAZAR_CHANNELS channel, ALAZAR_PARAMETERS parameter, long value) {
    RETURN_CODE ret = AlazarSetParameter(handle, channel, parameter, value);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

long Board::get_parameter(ALAZAR_CHANNELS channel, ALAZAR_PARAMETERS parameter) {
    long value;
    RETURN_CODE ret = AlazarGetParameter(handle, channel, parameter, &value);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
    return value;
}

void Board::set_record_size(unsigned pre_trigger_samples, unsigned post_trigger_samples) {
    RETURN_CODE ret = AlazarSetRecordSize(handle, pre_trigger_samples, post_trigger_samples);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

void Board::set_capture_clock(ALAZAR_CLOCK_SOURCES source,  ALAZAR_SAMPLE_RATES samplerate, ALAZAR_CLOCK_EDGES edge, int decimation) {
    RETURN_CODE ret = AlazarSetCaptureClock(handle, source, samplerate, edge, decimation);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

void Board::input_control(ALAZAR_CHANNELS channel, ALAZAR_COUPLINGS coupling, ALAZAR_INPUT_RANGES input_range, ALAZAR_IMPEDANCES impedance) {
    RETURN_CODE ret = AlazarInputControl(handle, channel, coupling, input_range, impedance);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

void Board::before_async_read(
    ALAZAR_CHANNELS channelSelect, 
    long transferOffset, unsigned transferLength, 
    unsigned recordsPerBuffer, unsigned recordsPerAcquisition, 
    unsigned flags) 
{
    RETURN_CODE ret = AlazarBeforeAsyncRead(handle, channelSelect, 
                                            transferOffset, transferLength,
                                            recordsPerBuffer, recordsPerAcquisition,
                                            flags);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}


void Board::start_capture() {
    RETURN_CODE ret = AlazarStartCapture(handle);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

void Board::abort_async_read() {
    RETURN_CODE ret = AlazarAbortAsyncRead(handle);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

void Board::post_async_buffer(void *buffer, unsigned buffer_size) {
    RETURN_CODE ret = AlazarPostAsyncBuffer(handle, buffer, buffer_size);
    if (ret!=ApiSuccess) {
        throw ATS_Exception(ret);
    }
}

RETURN_CODE Board::wait_async_buffer_complete(void *buffer, unsigned timeout) {
    RETURN_CODE ret = AlazarWaitAsyncBufferComplete(handle, buffer, timeout);
    if (ret!=ApiSuccess && ret!=ApiBufferNotReady && ret!=ApiBufferOverflow && ret!=ApiPciTimeout) {
        throw ATS_Exception(ret);
    }
    return ret;
}


} /* end namespace */