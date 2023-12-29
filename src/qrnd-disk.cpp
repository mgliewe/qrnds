
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "qrnd-core.h"
#include "qrnd-disk.h"

namespace QRND {

DiskReader::DiskReader(const char *name, const char *filename, int buffersize, int num_buffer) 
    : Producer(name, 1)
{
    allocate_buffers(buffersize, num_buffer);

    fd = open(filename, O_RDONLY);
    if (fd<0) {
        throw "cannot open input file for reading";
    }
}

DiskReader::~DiskReader() {
}

void DiskReader::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;

        Frame *frame = get_frame();
        uint8_t *data = (uint8_t *) frame->data;
        int sz = frame->capacity;
        while (sz>0) {
            int len = read(fd,data, sz);
            if (len<0) {
                close(fd);
                stop();
                break;
            } 
            data += len;
            sz -= len;
        }
        frame->count = data - (uint8_t *)frame->data;
        send(0, frame);
    }
}


ZeroReader::ZeroReader(const char *name, int buffersize, int num_buffer) 
    : DiskReader(name, "/dev/zero", buffersize, num_buffer)
{
}

ZeroReader::~ZeroReader() {
}

RandomReader::RandomReader(const char *name, int buffersize, int num_buffer) 
    : DiskReader(name, "/dev/random", buffersize, num_buffer)
{

}
RandomReader::~RandomReader() {
}

} // end namespace