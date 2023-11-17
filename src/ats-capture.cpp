#include <getopt.h>
#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>


#include "ATS-SDK.h"

using std::cout;
using std::cerr;
using std::endl;



const char *progname;
int verbose = 3;

char *infile = NULL;
char *outfile = NULL;



int system_id = 1;
int board_id = 1;
int channel = 2;
long samplerate = 3800L * 1000 * 1000;
int input_range = 1250;
int input_quadrant = 0;
bool eigth_bit = false;
bool pack_12_bit = false;
bool in_memory = false;
bool test_mode = false;
unsigned record_size = 8 * 1024;
unsigned record_count = 1024;
char *pyxdatfile = NULL;
char *pyxscriptfile = NULL;
bool start_pyxplot = false;

ATS::Board *board = 0;
unsigned int transfer_length;

/** a ring-list of buffers */
void init_buffer_list(int count);
void push_buffer(uint8_t *buffer);
uint8_t *pop_buffer();



enum options {
    OPT_NULL = 0,
    OPT_HELP,
    OPT_VERBOSE,
    OPT_OUTPUT,
    OPT_INPUT,
    OPT_IN_MEMORY,
    OPT_TEST,
    OPT_RECORD_SIZE,
    OPT_RECORD_COUNT,
    OPT_DEVICE,
    OPT_CHANNEL,
    OPT_TWO_CHANNEL,
    OPT_SAMPLERATE,
    OPT_INPUT_RANGE,
    OPT_EIGHT_BIT,
    OPT_PACK,
    OPT_PYXPLOT,
    OPT_PYXPLOT_SCRIPT,
    OPT_PYXPLOT_START,
    OPT_LIST_RATES,
    OPT_LIST_RANGES,
    OPT_LAST
};

struct option long_options[] = {
    { "help",           no_argument,        0, OPT_HELP },
    { "verbose",        optional_argument,  0, OPT_VERBOSE },
    { "list-rates",     no_argument,        0, OPT_LIST_RATES },
    { "list-ranges",    no_argument,        0, OPT_LIST_RANGES },

    { "output",         required_argument,  0, OPT_OUTPUT },
    { "input",          required_argument,  0, OPT_INPUT },

    { "in-memory",      no_argument,        0, OPT_IN_MEMORY },
    { "test",           no_argument,        0, OPT_TEST },
    { "record-size",    required_argument,  0, OPT_RECORD_SIZE },
    { "record-count",   required_argument,  0, OPT_RECORD_COUNT },
    { "device",         required_argument,  0, OPT_DEVICE },
    { "channel",        required_argument,  0, OPT_CHANNEL },
    { "two-channel",    no_argument,        0, OPT_CHANNEL },
    { "samplerate",     required_argument,  0, OPT_SAMPLERATE},
    { "input-range",    required_argument,  0, OPT_INPUT_RANGE},

    { "eight-bit",      no_argument,        0, OPT_EIGHT_BIT },
    { "pack",           no_argument,        0, OPT_PACK },

    { "pyxplot",        optional_argument,  0, OPT_PYXPLOT },
    { "pyxplot-script", optional_argument,  0, OPT_PYXPLOT_SCRIPT },
    { "pyxplot-start",  no_argument,        0, OPT_PYXPLOT_START },

    { NULL, 0, 0, 0 }

};

const char *short_options = "hvd:o:2c:8pmtr:n:s:f:F::S::";

void help()  {
    cerr << "usage: " << progname << " <options>" << endl
         << endl

         << "options:" << endl
         << "\t--help, -h                show this help" << endl
         << "\t--verbose, -v             verbose output" << endl 

         << "\t--config=<file>, -c       read configuration from <file>" << endl

         << "\t--output=<file>, -o       write data to <file>, use '-' for stdout" << endl
         << "\t--input=<file>            read from file instead of capturing" << endl

         << "\t--device=<dev>            device to capture from. Default: 1:1" << endl
         << "\t--channel=<c>             channel, default: channel 'B'" << endl
         << "\t--rate=<r>                sample rate, default 3.8G" << endl
         << "\t--two-channel             aquire channel A and B, store interleaved data" << endl
         << "\t--eight-bit               capture 8 bit per sample" << endl
         << "\t--pack                    enable 12-bit packing in output" << endl
         << "\t--in-memory               capture to memory before writing to disk" << endl
         << "\t--test                    just read data, dont write to disk" << endl
         << "\t--record-size=<n>         record size, default: 1024 samples" << endl
         << "\t--record-count=<n>        record count, use 0 for unlimited. Default: 0" << endl

         << "\t--list-ranges             show supported input voltage ranges" << endl
         << "\t--list-rates              show supported samplerates" << endl

         << "\t--pyxplot[=<file>]        generate data file suitable for pyxplot, append .dat if appropriate " << endl
         << "\t--pyxplot-script          generate pyxplot script file" << endl
         << endl

         << "Examples:" <<  endl
         << "\t# read 1G samples to binary file:" << endl
         << "\t]t" << progname << " -r 1k -n 1M -o output.bin" << endl
         << "\t# continously streaming to a pipe" << endl
         << "\t\t" << progname << " -r 64k -n 0 -o -" << endl
         << "\t# benchmarking thruput of 1Gsamples" << endl
         << "\t\ttime " << progname << "-v -r 1k -n 1M -m" << endl
         << "\t# show 1Msamples in pyxplot" << endl
         << "\t\t" << progname << " -r 1k -n 1k -F out.dat -S; pyplot out.pyxplot"
    ;
}

void usage()  {
    cerr << progname << ": aquire data from Alazartech digitizer board. Use --help for help" << endl;
}

int to_number(const char *str, unsigned base=1024, unsigned int factor = 1) {
    if (!isdigit(*str)) {
        throw std::runtime_error("bad number format");
    }
    char *endptr;
    float number = strtof(str, &endptr) * factor;
    switch(*endptr) {
        case 'm':
            number /= base; break;
        case 'k':
            number *= base; break;
        case 'M':
            number *= base*base; break;
        case 'G':
            number *= base*base*base; break;
        default:
            throw std::runtime_error("bad number format");
    }
    return number;
}

void parse_board_address(const char *addr, int &sys_id, int &board_id) {
    int sys, board;
    char *s;

    if (!isdigit(*addr))    
        throw std::runtime_error("board address");

    sys = strtol(addr, &s, 0);
    if (*s++!=':') 
        throw std::runtime_error("board address");
    if (!isdigit(*s))    
        throw std::runtime_error("board address");

    board=strtol(s, &s, 0);
    if (*s)
        throw std::runtime_error("board address");
 
    sys_id = sys;
    board_id = board;
}

void show_size(std::ostream &out, int sz) {
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

std::string size_str(unsigned long sz, int base=1024) {
    if (sz<base || sz%base!=0) {
        return std::to_string(sz);
    } else {
        sz /= base;
        if (sz<base  || sz%base!=0) {
            return std::to_string(sz) + "k";
        } else {
            sz /= base;
            if (sz<base  || sz%base!=0) {
                return std::to_string(sz) + "M";
            } else {
                sz /= base;
                return std::to_string(sz) + "G";
            }
        }
    }
}

void process_option(int short_option, const char *optarg) {
    switch(short_option) {
        case 'h': 
        case OPT_HELP: {
            help();
            exit(1);
        }

        case 'v': 
        case OPT_VERBOSE: {
            verbose ++;
            break;
        }

        case 'f': 
        case OPT_INPUT: {
            infile = strdup(optarg);
            break;
        }

        case 'd': 
        case OPT_DEVICE: {
            parse_board_address(optarg, system_id, board_id);
            break;
        }

        case 'o': 
        case OPT_OUTPUT: {
            outfile = strdup(optarg);
            break;
        }

        case '2': 
        case OPT_TWO_CHANNEL: {
            channel = 3;
            break;
        }

        case 'c': 
        case OPT_CHANNEL: {
            switch (*optarg) {
                case '0': case 'a': case 'A':
                    channel = 1; break;
                case '1': case 'b': case 'B':
                    channel = 2; break;
                default:
                    usage(); 
                    exit(0);
            }
            break;
        }

        case '8': 
        case OPT_EIGHT_BIT: {
            eigth_bit = true;
            break;
        }

        case 'p': 
        case OPT_PACK: {
            pack_12_bit = true;
            break;
        }

        case 'm': 
        case OPT_IN_MEMORY: {
            in_memory = true;
            break;
        }
        
        case 't': 
        case OPT_TEST: {
            test_mode = true;
            break;
        }

        case 'r': 
        case OPT_RECORD_SIZE: {
            record_size = to_number(optarg);
            break;
        }

        case 'n': 
        case OPT_RECORD_COUNT: {
            record_count = to_number(optarg);
            break;
        }

        case 'F': 
        case OPT_PYXPLOT: {
            if (optarg && *optarg) {
                pyxdatfile = strdup(optarg);
            } else {
                if (!outfile) {
                    cerr << "cannot derive pyxplot filename from output-file" << endl;
                    exit(0);
                }
                char *p = strrchr(outfile, '.');
                if (!p) {
                    cerr << "cannot derive pyxplot filename from output-file" << endl;
                    exit(0);
                }
                int len = p - outfile;
                pyxdatfile = (char *) malloc(len+4+1);
                strncpy(pyxdatfile, outfile, len);
                strcat(pyxdatfile, ".dat");
            }
            break;
        }

        case 'S': 
        case OPT_PYXPLOT_SCRIPT: {
            if (optarg && *optarg) {
                pyxscriptfile = strdup(optarg);
            } else {
                if (!pyxdatfile) {
                    cerr << "cannot derive pyxplot filename from pyxscript dat-file" << endl;
                    exit(0);
                }
                char *p = strrchr(pyxdatfile, '.');
                if (!p) {
                    cerr << "cannot derive pyxplot filename from pyxscript dat-file" << endl;
                    exit(0);
                }
                int len = p - pyxdatfile;
                pyxscriptfile = (char *) malloc(len+8+1);
                strncpy(pyxscriptfile, pyxdatfile, len);
                strcat(pyxscriptfile, ".pyxplot");
            }
            break;
        }

        case 'R': 
        case OPT_PYXPLOT_START: {
            start_pyxplot = true;
        }

        case OPT_LIST_RATES: {
            ATS::Board * board;
            try {
                board = new ATS::Board(system_id, board_id);
            } catch(...) {
                cout << "FATAL: no Alazar digitizer found." << endl;
                exit(1);
            }
            cout << "List of sample rates for board " << board->board_name() << " at [" 
                 << system_id << ":" << board_id <<"]:" << endl;
            const struct ATS::sample_rates *rates = ATS::sample_rates;
            while (rates->rate) {
                try {
                    board->set_capture_clock(INTERNAL_CLOCK, ATS::to_samplerate_code(rates->rate), CLOCK_EDGE_FALLING, 0);
                    cout << "  " << size_str(rates->rate, 1000) << "S/sec" << endl; 
                } catch(std::exception &e) {
                }
                rates++;
            }
            if (board) delete board;
            exit(1);
            break;
        }

        case OPT_LIST_RANGES: {
            ATS::Board * board;
            try {
                board = new ATS::Board(system_id, board_id);
            } catch(...) {
                cout << "FATAL: no Alazar digitizer found." << endl;
                exit(1);
            }
            cout << "List of input ranges for board " << board->board_name() << " at [" 
                 << system_id << ":" << board_id <<"]:" << endl;
            const struct ATS::input_ranges *ranges = ATS::input_ranges;
            while(ranges->code) {
                try {
                    board->input_control(CHANNEL_A, DC_COUPLING, ranges->code, IMPEDANCE_50_OHM);
                    if (ranges->q>0)
                        cerr << "+"; 
                    else if (ranges->q<0) 
                        cerr << "-";
                    if (ranges->mv % 1000 == 0) {
                        cerr << (ranges->mv/1000) << "V" << endl;
                    } else {
                        cerr << (ranges->mv) << "mV" << endl;
                    }
                } catch (...) {

                }
                ranges++;
            }
            if (board) delete board;
            exit(1);
            break;
        }

        default: {
            usage();
            exit(0);
        }
    }

}


void read_config(const char *file) {
    char line[120];
    FILE *cfg = fopen(file, "r");
    while (fgets(line, 120, cfg)) {
        char *p=line;

        // skip blanks
        while (p && isblank(*p)) 
            p++;

        // skip comments
        if (*p=='#')
            continue;

        // skip empty line
        if (!*p)
            continue;

        // read option identifier
        char *option = p;
        while (*p && !isblank(*p)) 
            p++;
        if (*p) *p++ = 0;

        // skip blanks for arg
        while (*p && isblank(*p)) 
            p++;

        // skip comment
        if (*p == '#') 
            *p = 0;

        // read optional argument
        char *arg = p;
        while (*p && !isblank(*p)) 
            p++;
        if (*p) *p++ = 0;


        struct option *opts = long_options;
        while (opts->name) {
            if (strcmp(option, opts->name)==0) {
                process_option(opts->val, arg);
                break;
            }
        }
    }
}

void write_config(std::ostream &out) {
    if (outfile)
        out << "output " << outfile << endl;
    if (infile)
        out << "input " << infile << endl;
    if (in_memory) 
        out << "in-memory" << endl;
    if (test_mode)
        out << "test" << endl;
    
    out << "device " << system_id << ":" << board_id << endl;
    out << "record-size " << record_size << endl;
    out << "record-count " << record_size << endl;
    out << "channel " << channel << endl;
    out << "samplerate " << samplerate << endl;
    out << "input-range " << ((input_quadrant<0)? "-" : (input_quadrant>0) ? "+" : "") 
        << input_range << endl;
    if (eigth_bit)
        out << "eight-bit" << endl;
    if (pack_12_bit)
        out << "pack";
    if (pyxdatfile)
        out << "pxplot " << pyxdatfile << endl;
    if (pyxscriptfile)
        out << "pxplot-script " << pyxscriptfile << endl;
    if (start_pyxplot)
        out << "pxplot-start " << endl;
}


int main(int argc, char * const argv[]) {
    int option_index;
    progname = argv[0];
    for (;; ){
        int opt = getopt_long(argc, argv, short_options, long_options, &option_index);
        if (opt==-1)
            break;
        else 
            process_option(opt, optarg);
    }


    if (outfile || pyxdatfile) {
        if (test_mode) {
            cerr << "FATAL: cannot create output in test-mode." << endl;
            return 1;
        }
    } else {
        if (!test_mode) {
            cerr << "FATAL: no output file." << endl;
            return 1;
        }        
    }

    if (in_memory || record_count==0) {
        cerr << "FATAL: cannot pre-allocate memory in continous streaming mode." << endl;
    }

    if (verbose>2) {
        if (infile) 
            cerr << "reading data from " << infile << endl;
        else {
            static const char * const channel_names[] = {
                "none", "A", "B", "A+B"
            };
            cerr << "Aquirering data form board [" << system_id << ',' << board_id << "]";
            cerr << " channel "<< channel_names[channel] << "." << endl;

            if (eigth_bit) {
                cerr << "Data-width is 8 bit." << endl;
            } else {
                cerr << "Data-width is 12 bit";
                if (pack_12_bit) {
                    cerr << ", data is packed to 12 bit";
                } 
                cerr << "." << endl;
            }
        }
        cerr << "Record size is " << size_str(record_size);
        if (record_count) {
            cerr << ", reading " << size_str(record_count) << " records." << endl;
        } else {
            cerr << ", streaming continously." << endl;
        }
        if (in_memory) {
            cerr << "Reading to preallocated Memory buffers." << endl;
        }
        if (test_mode) {
            cerr << "Running in test-mode." << endl;
        }
        if (outfile) {
            if (*outfile == '-')
                cerr << "streaming to stdout." << endl;
            else
                cerr << "writing data to " << outfile << "." << endl;
        }
        if (pyxdatfile) {
            cerr << "Generating pyxplot data-file to " << pyxdatfile;
            if (pyxscriptfile) {
                cerr << ", pyxplot script is written to " << pyxscriptfile;
            }
            cerr << "." << endl;
        }        
    }


    // allocate buffer
    uint8_t *memory_buffer;
    unsigned num_buffers = 8;
    transfer_length = (eigth_bit ? 1:2) * record_size;
    if (pack_12_bit)
        transfer_length * 2 / 3; 

    if (in_memory) {
        num_buffers = record_count;
    }

    memory_buffer = (uint8_t *) calloc(transfer_length, record_count); 
    if (!memory_buffer) {
        cerr << "FATAL: out of memory." << endl;
        return 1;
    }
    if (mlock(memory_buffer, transfer_length * record_count)<0) {
        cerr << "WARNING: could not lock memory buffer in physical memory. Try running as root." << endl;
    }

    init_buffer_list(num_buffers);
    uint8_t *memp = memory_buffer;
    for (int n=0; n<num_buffers; n++) {
        push_buffer(memp);
        memp += transfer_length;
    }


    int in_fd = -1, out_fd = -1;
    FILE *dat_fd = NULL;

    if (infile) {
        in_fd = open(infile, O_RDONLY);
        if (!in_fd) {
            cerr << "FATAL: cannot open " << infile << " for reading." << endl;
            return 1;
        }
    } else {
        try {

            board = new ATS::Board(system_id, board_id);

            board->abort_async_read();
            board->set_record_size(0, record_size);
            board->set_capture_clock(INTERNAL_CLOCK, ATS::to_samplerate_code(samplerate), CLOCK_EDGE_FALLING, 0);

            board->input_control(CHANNEL_A, DC_COUPLING, ATS::to_inputrange_code(input_quadrant, input_range), IMPEDANCE_50_OHM);
            board->input_control(CHANNEL_B, DC_COUPLING, ATS::to_inputrange_code(input_quadrant, input_range), IMPEDANCE_50_OHM);

            board->set_trigger_operation(
                TRIG_ENGINE_OP_J, 
                TRIG_ENGINE_J, TRIG_CHAN_A, TRIGGER_SLOPE_POSITIVE, 140,
                TRIG_ENGINE_K, TRIG_CHAN_B, TRIGGER_SLOPE_POSITIVE, 140
            );

            board->set_trigger_timeout(3);      // one tick is 10usec
            board->set_trigger_delay(0);

            board->before_async_read(
                CHANNEL_B,
                0, transfer_length,
                1, 0x7FFFFFFF,
                ADMA_CONTINUOUS_MODE | ADMA_FIFO_ONLY_STREAMING | ADMA_EXTERNAL_STARTCAPTURE
            );
            board->start_capture();

        } catch(std::exception &e) {
            cerr << e.what() << endl;
            cerr << "FATAL: failed to initialize digitizer board." << endl;
            return 1;
        }
    }

    // capture data
    int dat_counter = 1;    // datapoint counter for pyxplot
    uint8_t *buffer;
    for (int rec=0; record_count!=0 || rec<record_count; ++rec) {
        if (in_fd>=0) {
            buffer = pop_buffer();
            if(transfer_length!=read(in_fd, buffer, transfer_length)) {
                cerr << "FATAL: error in read from input." << endl;
            }
        } else {
            RETURN_CODE ret; 
            do {
                buffer = pop_buffer();
                // TODO: make timeout value adjustable?
                ret = board->wait_async_buffer_complete(buffer, 1);
                if (ret==ApiWaitTimeout) {
                    // wait for trigger and capture;
                    // note: the capturecard needs up to 5ms to startup...
                    push_buffer(buffer);
                } else if (ret==ApiBufferOverflow) {
                    // TODO: add flag to ignore overflows
                    throw std::runtime_error("on-board memory overflow");
                } 
            } while (ret!=ApiSuccess);
        }


        if (!test_mode && !in_memory) {
            if (out_fd>=0) {
                if (write(out_fd, buffer, transfer_length) != transfer_length) {
                    cerr << "FATAL: error on write to outfile" << endl;
                }
            }
            if (dat_fd) {
                if (eigth_bit) {
                    uint8_t * dat = buffer;
                    for (int i=0; i<record_count; ++i) {
                        fprintf(dat_fd, "%d   %d\n", dat_counter++, *dat++);
                    }
                } else if (pack_12_bit) {
                    uint8_t * dat = buffer;
                    for (int i=0; i<record_count; ++i) {
                        unsigned a = (*dat++) << 4;
                        unsigned x = *dat++;
                        unsigned b = *dat++;
                        a |= (x>>4) & 0x000f;
                        b |= (x<<8) & 0x0f00;
                        fprintf(dat_fd, "%d   %d\n", dat_counter++, a);
                        fprintf(dat_fd, "%d   %d\n", dat_counter++, b);
                    }
                } else {
                    uint16_t * dat = (uint16_t *)buffer;
                    for (int i=0; i<record_count; ++i) {
                        fprintf(dat_fd, "%d   %d\n", dat_counter++, *dat++);
                    }
                }
            }
        }
    }

    if (in_memory) {
        uint8_t *p = memory_buffer;
        int dat_counter = 1;
        for (int rec=0; rec<record_count; ++rec) {
            if (out_fd>=0) {
                if (write(out_fd, p, transfer_length) != transfer_length) {
                    cerr << "FATAL: error on write to outfile" << endl;
                }
            }
            if (dat_fd) {
                if (eigth_bit) {
                    uint8_t * dat = p;
                    for (int i=0; i<record_count; ++i) {
                        fprintf(dat_fd, "%d   %d\n", dat_counter++, *dat++);
                    }
                } else if (pack_12_bit) {
                    uint8_t * dat = p;
                    for (int i=0; i<record_count; ++i) {
                        unsigned a = (*dat++) << 4;
                        unsigned x = *dat++;
                        unsigned b = *dat++;
                        a |= (x>>4) & 0x000f;
                        b |= (x<<8) & 0x0f00;
                        fprintf(dat_fd, "%d   %d\n", dat_counter++, a);
                        fprintf(dat_fd, "%d   %d\n", dat_counter++, b);
                    }
                } else {
                    uint16_t * dat = (uint16_t *) p;
                    for (int i=0; i<record_count; ++i) {
                        fprintf(dat_fd, "%d   %d\n", dat_counter++, *dat++);
                    }
                }
            }
        }
    }

    if (!infile)
        board->abort_async_read();


    if (in_fd>=0) close(in_fd);
    if (out_fd>=0) close(out_fd);
    if (dat_fd) fclose(dat_fd);

    if (pyxscriptfile) {
        // TODO:
    }
}

uint8_t **buffer_list;
int buffer_list_index;
int buffer_list_count;
int buffer_list_capacity;

void init_buffer_list(int count) {
    buffer_list = (uint8_t **) calloc(sizeof(uint8_t *), count);
    buffer_list_capacity = count;
}

void push_buffer(uint8_t *buffer) {
    if (buffer_list_count==buffer_list_capacity) {
        throw std::runtime_error("bufferlist overflow");
    }
    int n = buffer_list_index + 1;
    if (n>buffer_list_capacity) n = 0;
    buffer_list[n] = buffer;
    buffer_list_count++;
}

uint8_t *pop_buffer() {
    if (buffer_list_count==0) {
        throw std::runtime_error("bufferlist underflow");
    }
    buffer_list_count--;

    buffer_list_index++;
    if (buffer_list_index>buffer_list_capacity) 
        buffer_list_index = 0;
    return buffer_list[buffer_list_index];
}






