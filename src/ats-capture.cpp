#include <getopt.h>
#include <iostream>
#include <fstream>
#include <cstring>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <assert.h>

#include "ATS-SDK.h"

using std::cout;
using std::cerr;
using std::endl;


const char *progname = NULL;
int verbose = 1;

char *infile = NULL;
char *outfile = NULL;

char *config_file = NULL;

int system_id = 1;
int board_id = 1;
int channel = 2;
long samplerate = 1800L * 1000 * 1000;
int input_range = 1250;
int input_quadrant = 0;
bool eigth_bit = false;
bool pack_12_bit = false;
bool drop_frames = false;
bool in_memory = false;
bool test_mode = false;
unsigned record_size = 8 * 1024;
unsigned record_count = 1024;

bool extract_entropy = false;
unsigned extract_inbits = 2048;
unsigned extract_outbits = 1152;
char *extract_seedfile = 0;

bool write_histogram = false;
bool histogram_binbits = 12;

char *pyxdatfile = NULL;


ATS::Board *board = 0;

/** a ring-list of buffers */
void init_buffer_list(int count);
void push_buffer(uint16_t *buffer);
uint16_t *pop_buffer();

extern "C" uint64_t builtin_extract_seed[];
extern "C" unsigned builtin_extract_seed_bits;

void extract(const unsigned N, uint64_t const *in, 
                    const unsigned L, uint64_t *out, 
                    uint64_t const *m);

enum options {
    OPT_NULL = 0,
    OPT_HELP,
    OPT_VERBOSE,
    OPT_QUIET,
    OPT_READ_CONFIG,
    OPT_WRITE_CONFIG,
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
    OPT_DROPFRAMES,
    OPT_PYXPLOT,
    OPT_LIST_RATES,
    OPT_LIST_RANGES,
    OPT_EXTRACT,
    OPT_INBITS,
    OPT_OUTBITS,
    OPT_SEEDFILE,
    OPT_HISTOGRAM,
    OPT_BINBITS,
    OPT_LAST
};

struct option long_options[] = {
    { "help",           no_argument,        0, OPT_HELP },
    { "verbose",        optional_argument,  0, OPT_VERBOSE },
    { "quiet",          no_argument,        0, OPT_QUIET },

    { "config",         required_argument,  0, OPT_READ_CONFIG },
    { "write-config",   required_argument,  0, OPT_WRITE_CONFIG },

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
    { "drop-frames",    no_argument,        0, OPT_DROPFRAMES },

    { "extract-entropy", no_argument,       0, OPT_EXTRACT },
    { "in-bit-count",   required_argument,  0, OPT_INBITS },
    { "out-bit-count",  required_argument,  0, OPT_OUTBITS },
    { "seed-file",      required_argument,  0, OPT_SEEDFILE },
    { "histogram",      required_argument,  0, OPT_HISTOGRAM },

    { "pyxplot",        optional_argument,  0, OPT_PYXPLOT },

    { NULL, 0, 0, 0 }

};

const char *short_options = "hvf:o:i:D:C:R:28pmtr:n:y::";

void help()  {
    cerr << "usage: " << progname << " <options>" << endl
         << endl

         << "options:" << endl
         << "\t--help, -h                show this help" << endl
         << "\t--verbose=<level>, -v     verbose output. Use -vv to be even most verbose." << endl 
         << "\t--quiet, -q               no diagnostic output. Same as --verbose=0." << endl

         << "\t--config=<file>, -f       read configuration from <file>" << endl
         << "\t--write-config=<file>     write used configuration to <file>" << endl

         << "\t--output=<file>, -o       write data to <file>, use '-' for stdout" << endl
         << "\t--input=<file>, -i        read from file instead of capturing" << endl

         << "\t--device=<dev>, -D        device to capture from. Default: 1:1" << endl
         << "\t--channel=<c> , -C        channel, default: channel 'B'" << endl
         << "\t--rate=<r>, -R            sample rate, default 3.8G" << endl
         << "\t--two-channel, -2         aquire channel A and B, store interleaved data" << endl
         << "\t--eight-bit, -8           capture 8 bit per sample" << endl
         << "\t--pack, -p                enable 12-bit packing in output" << endl
         << "\t--in-memory, -m           capture to memory before writing to disk" << endl
         << "\t--test, -t                just read data, dont write to disk" << endl
         << "\t--record-size=<n>, -r     record size, default: 1024 samples" << endl
         << "\t--record-count=<n>, -n    record count, use 0 for unlimited. Default: 0" << endl
         << "\t--drop-frames             ignore buffer-overflow errors while capturing" << endl 

         << "\t--extract-entropy         extract entropy using toeplitz matrix" << endl
         << "\t--in-bit-count=<n>        number of input bits for --extract-entropy. Default: 2048" << endl
         << "\t--out-bit-count=<n>       number of output bits for --extract-entropy. Default: 1152" << endl
         << "\t--seed-file=<file>        random seed used for --extract-entropy" << endl

         << "\t--histogram               output value distribution diagram instread of data" << endl
         << "\t--bin-bits                number of highest significant bits for --histogram. Default: 12" << endl

         << "\t--list-ranges             show supported input voltage ranges" << endl
         << "\t--list-rates              show supported samplerates" << endl

         << "\t--pyxplot[=<file>], -y    generate data file suitable for pyxplot, append .dat if appropriate " << endl
         << endl

         << "Examples:" <<  endl
         << "\t# read 1G samples to binary file:" << endl
         << "\t]t" << progname << " -r 1k -n 1M -o output.bin" << endl
         << "\t# continously streaming to a pipe" << endl
         << "\t\t" << progname << " -r 64k -n 0 -o -" << endl
         << "\t# benchmarking thruput of 1Gsamples" << endl
         << "\t\ttime " << progname << "-v -r 1k -n 1M -m" << endl
         << "\t# show 1M samples in pyxplot" << endl
         << "\t\t" << progname << " -r 1k -n 1k -F out.dat -S; pyxplot out.pyxplot"
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
        case 0:
            break;
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
            if (optarg && *optarg) {
                switch (*optarg) {
                case '0':       verbose = 0; break;
                case '1':       verbose = 1; break;
                case '2':       verbose = 2; break;
                case '3':       verbose = 3; break;
                case 'v':       verbose = 3; break;
                default:
                    throw std::runtime_error("verbose level must be between 0 and 3");
                }
            } else {
                verbose ++;
            }
            break;
        }

        case OPT_READ_CONFIG: {
            void read_config(const char *file);
            read_config(optarg);
            break;
        }

        case OPT_WRITE_CONFIG: {
            config_file = strdup(optarg);

            int ret = access(config_file, R_OK);
            if (ret) {
                throw std::runtime_error("cannot create config file: "+std::string(strerror(errno)));
            }
            
            break;
        }

        case 'i': 
        case OPT_INPUT: {
            infile = strdup(optarg);
            if (strcmp(outfile, "-") != 0) {
                int ret = access(extract_seedfile, R_OK);
                if (ret) {
                    throw std::runtime_error("cannot locate input file: "+std::string(strerror(errno)));
                }
            }
            break;
        }

        case 'D': 
        case OPT_DEVICE: {
            parse_board_address(optarg, system_id, board_id);
            break;
        }

        case 'o': 
        case OPT_OUTPUT: {
            outfile = strdup(optarg);
            if (strcmp(outfile, "-") != 0) {
                int ret = access(extract_seedfile, R_OK);
                if (ret) {
                    throw std::runtime_error("cannot create output file: "+std::string(strerror(errno)));
                }
            }
            break;
        }

        case '2': 
        case OPT_TWO_CHANNEL: {
            channel = 3;
            break;
        }

        case 'C': 
        case OPT_CHANNEL: {
            switch (*optarg) {
                case '0': case 'a': case 'A':
                    channel = 1; break;
                case '1': case 'b': case 'B':
                    channel = 2; break;
                default:
                    throw std::runtime_error("Bad channel: " + std::string(optarg));
            }
            break;
        }

        case '8': 
        case OPT_EIGHT_BIT: {
            eigth_bit = true;
            if (pack_12_bit) {
                throw std::runtime_error("--eight-bit and --pack are mutual exclusive");
            }
            break;
        }

        case 'p': 
        case OPT_PACK: {
            pack_12_bit = true;
            break;
        }

        case OPT_DROPFRAMES: {
            drop_frames = true;
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

        case OPT_EXTRACT: {
            extract_entropy = true;
            break;
        }

        case OPT_INBITS: {
            extract_inbits = to_number(optarg);
            break;
        }

        case OPT_OUTBITS: {
            extract_outbits = to_number(optarg);
            break;
        }

        case OPT_SEEDFILE: {
            extract_seedfile = strdup(optarg);
            int ret = access(extract_seedfile, R_OK);
            if (ret) {
                throw std::runtime_error("cannot locate seedfile: "+std::string(strerror(errno)));
            }
            break;
        }

        case OPT_HISTOGRAM: {
            write_histogram = true;
            break;
        }

        case OPT_BINBITS: {
            histogram_binbits = to_number(optarg);
            if (histogram_binbits<4 || histogram_binbits>12) {
                throw std::runtime_error("bin-bits bust be between 4 and 12.");
            }
            break;
        }

        case 'y': 
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

                int ret = access(extract_seedfile, W_OK);
                if (ret) {
                    throw std::runtime_error("cannot create .dat-file: "+std::string(strerror(errno)));
                }
            }
            break;
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

void write_config(const char *filename) {
    std::fstream out = std::fstream(filename, std::fstream::out);
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
    if (drop_frames) {
        out << "drop-frames" << endl;
    }
    if (extract_entropy) {
        out << "extract-entropy" << endl;
        out << "in-bit-count " << extract_inbits << endl;
        out << "out-bit-count " << extract_outbits << endl;
        out << "seed-file " << extract_seedfile << endl;
    }

    if (write_histogram) {
        out << "histogram" << endl;
    }

    if (pyxdatfile)
        out << "pxplot " << pyxdatfile << endl;

    out.close();
}


int main(int argc, char * const argv[]) {
    int option_index;

    progname = argv[0];
    if (argc==1) {
        usage();
        exit(0);
    }

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

    if (extract_entropy) {
        if (extract_inbits<extract_outbits) {
            throw std::runtime_error("--out-bit-count must be less or equal --in-bit-count");
        }
        if (extract_inbits == 0 || extract_inbits%64 != 0) {
            throw std::runtime_error("--in-bit-count must be dividable by 64");
        }
        if (extract_outbits == 0 || extract_outbits%64 != 0) {
            throw std::runtime_error("--out-bit-count must be dividable by 64");
        }
        if (record_size < extract_inbits) {
            throw std::runtime_error("--in-bit-count must be less than --record-size");
        }
        if (record_size%extract_inbits != 0) {
            throw std::runtime_error("--record_size must be dividable by --in-bit-count");
        }
    }

    if (verbose>1) {
        if (infile) {
            cerr << "Reading data from ";
            if (strcmp(infile, "-")==0)
                cerr << "stdin";
            else
                cerr << infile;
            cerr << "." << endl;
        } else {
            static const char * const channel_names[] = {
                "none", "A", "B", "A+B"
            };
            cerr << "Capturing data form board [" << system_id << ',' << board_id << "]";
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
            if (strcmp(outfile, "-")==0)
                cerr << "streaming to stdout." << endl;
            else
                cerr << "writing data to " << outfile << "." << endl;
        }
        if (pyxdatfile) {
            cerr << "Generating pyxplot data-file to " << pyxdatfile;
            cerr << "." << endl;
        }        
    }

    if (config_file) {
        write_config(config_file);
    }

    // allocate buffer
    uint8_t *memory_buffer;
    unsigned num_buffers = 8;
    size_t buffer_size = (eigth_bit ? 1:2) * record_size;
    if (pack_12_bit)
        buffer_size * 2 / 3; 


    uint16_t **in_memory_buffers = NULL;
    uint16_t **in_memory_p = NULL;
    if (in_memory) {
        num_buffers = record_count;
        in_memory_p = in_memory_buffers = (uint16_t **) calloc(sizeof (uint16_t *), num_buffers);
    }

    init_buffer_list(num_buffers);

    // open disk files
    int in_fd = -1, out_fd = -1;
    FILE *dat_fd = NULL;


    if (infile) {       // open input file and allocate buffers
        in_fd = open(infile, O_RDONLY);
        if (!in_fd) {
            throw std::runtime_error(std::string("cannot open ") + infile + " for reading: " + std::string(strerror(errno)));
        }

        for (int n=0; n<num_buffers; ++n) {
            uint16_t *buffer = (uint16_t *) malloc(buffer_size);
            push_buffer(buffer);
        }   
 
    } else {            // open capture device
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
                0, record_size,
                1, 0x7FFFFFFF,
                ADMA_CONTINUOUS_MODE | ADMA_FIFO_ONLY_STREAMING | ADMA_EXTERNAL_STARTCAPTURE
            );

            for (int n=0; n<num_buffers; ++n) {
                uint16_t *buffer = board->alloc_buffer(buffer_size);
                board->post_async_buffer(buffer, buffer_size);
                push_buffer(buffer);
            }   

            board->start_capture();

        } catch(std::exception &e) {
            cerr << e.what() << endl;
            std::throw_with_nested(std::runtime_error("failed to initialize digitizer board."));
        }
    }

    if (outfile) {      // open output file
        out_fd = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0666);
        if (out_fd<0) {
            throw std::runtime_error("failed to create output file " + std::string(outfile) + ": " + std::string(strerror(errno)));
        }
    }

    // capture data
    int dat_counter = 1;    // datapoint counter for pyxplot
    int dat_increment = 1;

    unsigned *histogram;
    unsigned histogram_shift = 0;
    if (write_histogram) {
        if (eigth_bit) {
            unsigned histogram_size = 0x0100;
            if (histogram_binbits<8) {
                for (int n=8; n>histogram_binbits; --n) {
                    histogram_shift++;
                    dat_increment <<= 1;
                    histogram_size >>= 1;
                }
                histogram = (unsigned *) calloc(sizeof(unsigned), histogram_size);
            } 
            histogram = (unsigned *) calloc(sizeof(unsigned), 0x0100);
            dat_counter = -0x0080;
        } else {
            unsigned histogram_size = 0x1000;
            if (histogram_binbits<12) {
                for (int n=12; n>histogram_binbits; --n) {
                    histogram_shift++;
                    dat_increment <<= 1;
                    histogram_size >>= 1;
                }
            }
            histogram = (unsigned *) calloc(sizeof(unsigned), 0x1000);
            dat_counter = -0x0800;
        }
    }

    uint16_t *buffer;
    for (int rec=0; record_count==0 || rec<record_count; ++rec) {
        if (in_fd>=0) {
            buffer = pop_buffer();
            if(buffer_size!=read(in_fd, buffer, buffer_size)) {
                throw std::runtime_error("error on read from input file: " + std::string(strerror(errno)));
            }
        } else {
            RETURN_CODE ret; 
            do {
                buffer = pop_buffer();
                // TODO: make timeout value adjustable?
                ret = board->wait_async_buffer_complete(buffer, 10);
                if (ret==ApiWaitTimeout) {
                    // wait for trigger and capture;
                    // note: the capturecard needs up to 5ms to startup...

                    // TODO: add flag to ignore overflows
                    throw std::runtime_error("timeout waiting for capture data");

                } else if (ret==ApiBufferOverflow) {

                    // TODO: add flag to ignore overflows
                    throw std::runtime_error("on-board memory overflow");
                } 
            } while (ret!=ApiSuccess);
        }

        int output_size = buffer_size;

        if (in_memory) {
            *in_memory_p++ = buffer;

        } else {
            if (extract_entropy) {

            }
            
            if (write_histogram) {
                if (eigth_bit) {
                    int8_t * dat = (int8_t *) buffer;
                    for (int i=0; i<output_size; i+=2) {
                        int32_t v = *dat;
                        v += 0x0800;
                        v &= 0x0fff;
                        v >>= histogram_shift;
                        histogram[v] ++;
                    }
                } else if (pack_12_bit) {
                    uint8_t * dat = (uint8_t *) buffer;
                    for (int i=0; i<output_size; i+=3) {
                        int32_t a = (*dat++) << 4;
                        int32_t x = *dat++;
                        int32_t b = *dat++;
                        a |= (x>>4) & 0x000f;
                        b |= (x<<8) & 0x0f00;
                        a += 0x0800;
                        a &= 0x0fff;
                        a >>= histogram_shift;
                        histogram[a] ++;
                        b += 0x0800;
                        b &= 0x0fff;
                        b >>= histogram_shift;
                        histogram[b] ++;
                    }
                } else {
                    uint16_t * dat = (uint16_t *)buffer;
                    for (int i=0; i<output_size; i+=2) {
                        int v = *dat;
                        v += 0x0800;
                        v &= 0xfff;
                        v >>= histogram_shift;
                        histogram[v] ++;
                    }
                }
            }
        }

        if (!test_mode && !write_histogram && !in_memory) {
            if (out_fd>=0) {
                fprintf(stderr, "%d\r", rec); fflush(stderr);
                if (write(out_fd, buffer, output_size) != output_size) {
                    throw std::runtime_error("error on write to outfile: " + std::string(strerror(errno)));
                }
            }
            if (dat_fd) {
                if (eigth_bit) {
                    uint8_t * dat = (uint8_t *) buffer;
                    for (int i=0; i<output_size; i+=2) {
                        fprintf(dat_fd, "%d   %d\n", dat_counter, *dat++);
                        dat_counter += dat_increment;
                    }
                } else if (pack_12_bit) {
                    uint8_t * dat = (uint8_t *) buffer;
                    for (int i=0; i<output_size; i+=3) {
                        int32_t a = (*dat++) << 4;
                        int32_t x = *dat++;
                        int32_t b = *dat++;
                        a |= (x>>4) & 0x000f;
                        b |= (x<<8) & 0x0f00;
                        fprintf(dat_fd, "%d   %d\n", dat_counter, a);
                        dat_counter += dat_increment;
                        fprintf(dat_fd, "%d   %d\n", dat_counter, b);
                        dat_counter += dat_increment;
                    }
                } else {
                    uint16_t * dat = (uint16_t *)buffer;
                    for (int i=0; i<output_size; i+=2) {
                        fprintf(dat_fd, "%d   %d\n", dat_counter, *dat++);
                        dat_counter += dat_increment;
                    }
                }
            }
        }

        push_buffer(buffer);
        if (in_fd<0) 
            board->post_async_buffer(buffer, buffer_size);
    }

    if (in_memory) {
        // write collected data frames
        //TODO: implement
    }

    if (!infile)
        board->abort_async_read();


    if (in_fd>=0) close(in_fd);
    if (out_fd>=0) close(out_fd);
    if (dat_fd) fclose(dat_fd);

}

void write_buffer(uint8_t *data, unsigned size) {

}



void extract(const unsigned N, uint64_t const *in, 
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

uint16_t **buffer_list;
int buffer_list_index;
int buffer_list_count;
int buffer_list_capacity;

void init_buffer_list(int count) {
    buffer_list = (uint16_t **) calloc(sizeof(uint8_t *), count);
    buffer_list_capacity = count;
}

void push_buffer(uint16_t *buffer) {
    if (buffer_list_count==buffer_list_capacity) {
        throw std::runtime_error("bufferlist overflow");
    }
    int n = buffer_list_index + 1;
    if (n>buffer_list_capacity) n = 0;
    buffer_list[n] = buffer;
    buffer_list_count++;
}

uint16_t *pop_buffer() {
    if (buffer_list_count==0) {
        throw std::runtime_error("bufferlist underflow");
    }
    buffer_list_count--;

    buffer_list_index++;
    if (buffer_list_index>buffer_list_capacity) 
        buffer_list_index = 0;
    return buffer_list[buffer_list_index];
}






