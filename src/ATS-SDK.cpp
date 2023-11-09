

#include <stdio.h>
#include "ATS-SDK.h"

namespace ATS {

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


Board::Board(int system_id, int board_id) {
    handle = AlazarGetBoardBySystemID(system_id, board_id);
    if (handle==NULL) {
        throw "Alazar Capture board was not found";
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

}