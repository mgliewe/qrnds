
#include <iostream>

#include "ATS-SDK.h"


using std::cout, std::endl;


void show_size(std::ostream &out, unsigned long sz) {
    if (sz<1024) {
        out << sz << "B";
    } else {
        sz /= 1024;
        if (sz<1024) {
            out << sz << "kB";
        } else {
            sz /= 1024;
            if (sz<1024) {
                out << sz << "MB";
            } else {
                sz /= 1024;
                out << sz << "GB";
            }
        }
    }
}

const char *yesno(int v) {
    return v?"yes":"no";
}

int main() {

    cout << "ATS-SDK version:" << ATS::sdk_version() <<endl; 
    cout << "Kernel driver version:" << ATS::sdk_version() <<endl; 

    if (ATS::num_boards()<1) {
        cout << "no digitizer cads found." << endl;
    }

    for( int sysid=1; sysid<=ATS::num_systems(); ++sysid) {
        for (int boardid = 1; boardid<=ATS::num_boards(sysid); ++boardid) {

            ATS::Board board(sysid, boardid);

            cout << "Found " << board.board_name() << " board on [" 
                 << sysid <<',' << boardid << ']' << endl;  

            cout << "\tSerial number: 0x" << std::hex
                << board.query_capability(GET_SERIAL_NUMBER) << std::dec << endl;
            cout << "\tBoard revision: " 
                << board.board_revision() << endl;
            cout << "\tFPGA type: " 
                << (board.query_capability(GET_CPF_DEVICE)==1 ? "SL50":"SL260") << endl;
            cout << "\tCPLD version: " 
                << board.cpld_version() << endl;

            cout << "\tOn-Board memory: "; 
                show_size(cout, board.onboard_memory); 
                cout << endl;
            cout << "\tBits per sample: " << board.bits_per_sample << endl;

            cout << "\t8-bit packing support: "
                 << yesno(board.query_capability(CAP_SUPPORT_8_BIT_PACKING)) << endl;
            cout << "\t12-bit packing support: "
                 << yesno(board.query_capability(CAP_SUPPORT_12_BIT_PACKING)) << endl;

            cout << "native single-port aquisition: "
                 << yesno(board.query_capability(CAP_SUPPORTS_NATIVE_SINGLE_PORT)) << endl;
//            cout << "streaming DMA support: "
//                 << yesno(board.query_capability(OPTION_STREAMING_DMA)) << endl;

            cout << "PCI link speed: " << board.query_capability(GET_PCIE_LINK_SPEED) << endl; 
            cout << "PCI link width: " << board.query_capability(GET_PCIE_LINK_WIDTH) << endl;
        }
    }

    return 1;
}