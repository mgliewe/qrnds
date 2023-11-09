#include <getopt.h>
#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>


using std::cout, std::cerr, std::endl;


struct option long_options[] = {
    { "help",           no_argument,        0, 'h' },
    { "output",         required_argument,  0, 'o' },
    { "input",          required_argument,  0, 'f' },

    { "device",         required_argument,  0, 'd' },
    { "channel",        required_argument,  0, 'c' },
    { "two-channel",    no_argument,        0, '2' },
    { "eight-bit",      no_argument,        0, '8' },
    { "pack",           no_argument,        0, 'p' },
    { "in-memory",      no_argument,        0, 'm' },
    { "test",           no_argument,        0, 't' },
    { "record-size",    required_argument,  0, 'r' },
    { "record-count",   required_argument,  0, 'n' },
    { "strategy",       required_argument,  0, 's' },

    { "pyxplot",        optional_argument,  0, 'F' },
    { "pyxplot-script", optional_argument,  0, 'S' },

    { NULL, 0, 0, 0 }

};

const char *short_options = "hvo:2c:8pmtr:n:s:f:F::S::";

void help(const char *cmd)  {
    cerr << "usage: " << cmd << " <options>" << endl
         << endl

         << "options:" << endl
         << "\t--help, -h                show this help" << endl
         << "\t--verbose, -v             verbose output" << endl 
         << "\t--output <file>, -o       write data to <file>, use '-' for stdout" << endl

         << "\t input=<file>, -f         read from file instead of capturing" << endl

         << "\t--device <dev>, -d        device to capture from. Default: 0:0" << endl
         << "\t--channel <c>, -c         channel, default: channel 'B'" << endl
         << "\t--two-channel, -2         aquire channel A and B, store interleaved data" << endl
         << "\t--eight-bit, -8           capture 8 bit per sample" << endl
         << "\t--pack, -p                enable 12-bit packing in output" << endl
         << "\t--in-memory, -m           capture to memory before writing to disk" << endl
         << "\t--test, -t                just read data, dont write to disk" << endl
         << "\t--record-size <n>, -r     record size, default: 1024 samples" << endl
         << "\t--record-count <n>, -n    record count, use 0 for unlimited. Default: 0" << endl
         << "\t--strategy <n>, -s        aquisition strategy. Use one of:" << endl
         << "\t                              P - use polling" << endl
         << "\t                              D - traditional DMA" << endl
         << "\t                              A - asynchronious DMA (default)" << endl
         << "\t                              S - asynchronious DMA streaming" << endl

         << "\t--pyxplot [<file>], -F    generate data file suitable for pyxplot, append .dat if appropriate " << endl
         << "\t--pyxplot-script, -S      generate pyxplot script file" << endl
         << endl

         << "Examples:" <<  endl
         << "\t# read 1G samples to binary file:" << endl
         << "\t]t" << cmd << " -r 1k -n 1M -o output.bin" << endl
         << "\t# continously streaming to a pipe" << endl
         << "\t\t" << cmd << " -r 64k -n 0 -o -" << endl
         << "\t# benchmarking thruput of 1Gsamples" << endl
         << "\t\ttime " << cmd << "-v -r 1k -n 1M -m" << endl
         << "\t# show 1Msamples in pyxplot" << endl
         << "\t\t" << cmd << " -r 1k -n 1k -F out.dat -S; pyplot out.pyxplot"
    ;
}

void usage(const char *cmd)  {
    cerr << cmd << ": aquire data from Alazartech digitizer board. Use --help for help" << endl;
}


enum strategy {
    polling = 0, traditional_dma=1, async_dma=2, streaming_dma=3
} strategy = async_dma;

int verbose = 0;
char *infile = NULL;
char *outfile = NULL;
int system_id = 0;
int board_id = 0;
int channel = 2;
bool eigth_bit = false;
bool pack_12_bit = false;
bool in_memory = false;
bool test_mode = false;
unsigned record_size = 1024;
unsigned record_count = 1024;
char *pyxdatfile = NULL;
char *pyxscriptfile = NULL;

bool init_hardware();
uint8_t * read_buffer(uint8_t *buffer);
bool release_hardware();


int to_number(const char *str) {
    if (!isdigit(*str)) {
        throw "bad number format";
    }
    char *endptr;
    long number = strtol(str, &endptr, 0);
    switch(*endptr) {
        case 'k': case 'K':
            number *= 1024; break;
        case 'm': case 'M':
            number *= 1024*1024; break;
        case 'g': case 'G':
            number *= 1024*1024*1024; break;
        default:
        throw "bad number format";
    }
    return number;
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


int main(int argc, char * const argv[]) {
    int option_index;

    for (;; ){

        int opt = getopt_long(argc, argv, short_options, long_options, &option_index);
        if (opt==-1)
            break;

        switch(opt) {
            case 'h': {
                help(argv[0]);
                return 1;
            }

            case 'v': {
                verbose ++;
                break;
            }

            case 'f': {
                infile = strdup(optarg);
                break;
            }

            case 'd': {
                // TODO:
            }

            case 'o': {
                outfile = strdup(optarg);
                break;
            }

            case '2': {
                channel = 3;
                break;
            }

            case 'c': {
                switch (*optarg) {
                    case '0': case 'a': case 'A':
                        channel = 1; break;
                    case '1': case 'b': case 'B':
                        channel = 2; break;
                    default:
                        usage(argv[0]); return 0;
                }
                break;
            }

            case '8': {
                eigth_bit = true;
                break;
            }

            case 'p': {
                pack_12_bit = true;
                break;
            }

            case 'm': {
                in_memory = true;
                break;
            }
            
            case 't': {
                test_mode = true;
                break;
            }

            case 'r': {
                record_size = to_number(optarg);
                break;
            }

            case 'n': {
                record_count = to_number(optarg);
                break;
            }

            case 's': {
                switch (*optarg) {
                    case 'p': case 'P': case '0':
                        strategy = polling; break;
                    case 'd': case 'D': case '1':
                        strategy = traditional_dma; break;
                    case 'a': case 'A': case '2':
                        strategy = async_dma; break;
                    case 's': case 'S': case '3':
                        strategy = streaming_dma; break;
                    default:
                        usage(argv[0]); return 0;
                }
            }

            case 'F': {
                if (optarg && *optarg) {
                    pyxdatfile = strdup(optarg);
                } else {
                    if (!outfile) {
                        cerr << "cannot derive pyxplot filename from output-file" << endl;
                        return 0;
                    }
                    char *p = strrchr(outfile, '.');
                    if (!p) {
                        cerr << "cannot derive pyxplot filename from output-file" << endl;
                        return 0;
                    }
                    int len = p - outfile;
                    pyxdatfile = (char *) malloc(len+4+1);
                    strncpy(pyxdatfile, outfile, len);
                    strcat(pyxdatfile, ".dat");
                }
                break;
            }

            case 'S': {
                if (optarg && *optarg) {
                    pyxscriptfile = strdup(optarg);
                } else {
                    if (!pyxdatfile) {
                        cerr << "cannot derive pyxplot filename from pyxscript dat-file" << endl;
                        return 0;
                    }
                    char *p = strrchr(pyxdatfile, '.');
                    if (!p) {
                        cerr << "cannot derive pyxplot filename from pyxscript dat-file" << endl;
                        return 0;
                    }
                    int len = p - pyxdatfile;
                    pyxscriptfile = (char *) malloc(len+8+1);
                    strncpy(pyxscriptfile, pyxdatfile, len);
                    strcat(pyxscriptfile, ".pyxplot");
                }
                break;
            }

            default: {
                usage(argv[0]);
                return 0;
            }
        }
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

    if (verbose>2) {
        if (infile) 
            cerr << "reading date from " << infile << endl;
        else {
            static const char * const strategy_names[] = {
                "polling", "traditional dma", "asynchronious dma", "asynchronious streaming dma"
            };
            static const char * const channel_names[] = {
                "none", "A", "B", "A+B"
            };
            cerr << "Aquirering data form board [" << system_id << ',' << board_id << "]";
            cerr << " channel "<< channel_names[channel] << "." << endl;
            cerr << "using " << strategy_names[strategy] << " strategy." << endl;
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
        cerr << "Record size is "; show_size(cerr, record_size);
        if (record_count) {
            cerr << ", reading "; show_size(cerr, record_count); cerr << " records." << endl;
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


    uint8_t *memory_buffer;
    //TODO: 12bit packing!
    int rec_size = (eigth_bit ? 1:2) * record_size;

    if (test_mode) {
        memory_buffer = (uint8_t *) calloc(rec_size, 1); 
        if (!memory_buffer) {
            cerr << "FATAL: out of memory." << endl;
            return 1;
        }
    } else if (in_memory) {
        memory_buffer = (uint8_t *) calloc(rec_size, record_count); 
        if (!memory_buffer) {
            cerr << "FATAL: out of memory." << endl;
            return 1;
        }
        if (mlock(memory_buffer, rec_size * record_count)<0) {
            cerr << "FATAL: could not lock memory buffer in physical memory. Try running as root." << endl;
            return 1;
        }
    } else {
        memory_buffer = (uint8_t *) calloc(rec_size, 1); 
        if (!memory_buffer) {
            cerr << "FATAL: out of memory." << endl;
            return 1;
        }
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
        if (!init_hardware()) {
            cerr << "FATAL: failed to initialize digitizer board." << endl;
            return 1;
        }
    }

    uint8_t *mem_p = memory_buffer;
    int dat_counter = 1;
    for (int rec=0; rec<record_count; ++rec) {
        uint8_t *p;
        if (in_fd>=0) {
            if(rec_size!=read(in_fd, mem_p, rec_size)) {
                cerr << "FATAL: error in read from input." << endl;
            }
            p = mem_p;
        } else { 
            p = read_buffer(mem_p);
            if (!p) {
                cerr << "FATAL: error on captureing." << endl;
            }
        }
        if (in_memory) {
            mem_p += rec_size;
        }

        if (!test_mode && !in_memory) {
            if (out_fd>=0) {
                if (write(out_fd, p, rec_size) != rec_size) {
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
                    uint16_t * dat = (uint16_t *)p;
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
                if (write(out_fd, p, rec_size) != rec_size) {
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

            p += rec_size;
        }
    }

    release_hardware();

    if (in_fd>=0) close(in_fd);
    if (out_fd>=0) close(out_fd);
    if (dat_fd) fclose(dat_fd);

    if (pyxscriptfile) {
        // TODO:
    }
}


bool init_hardware() { return false; }
uint8_t * read_buffer(uint8_t *buffer) { return NULL; }
bool release_hardware() { return false; }
