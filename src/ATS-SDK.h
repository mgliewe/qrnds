#include "../include/AlazarApi.h"

#include <stdint.h>

namespace ATS {

class Board {
    HANDLE handle;
public:
    int system_id, board_id;
    long onboard_memory;
    int bits_per_sample;
    int num_channels;

    Board(int system_id, int board_id);

    int board_kind();
    const char *board_name();
    const char *board_revision();
    const char *cpld_version();

    uint32_t query_capability(ALAZAR_CAPABILITIES cap);

};

const char * driver_version();
const char * sdk_version();
int num_boards();
int num_systems();
int num_boards(int system_id);

}
