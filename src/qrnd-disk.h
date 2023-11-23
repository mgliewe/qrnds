
#ifndef __QRND_DISK_H__
#define __QRND_DISK_H__

#include "qrnd-core.h"

/**
 * A producer reading from disk
 */
class DiskReader : public Producer, public Runnable {
private:
    int fd;
protected:
    void run();
public:
    DiskReader(const char *name, const char *filename, int buffersize = 8*K, int num_buffer = 8);
    DiskReader(std::string &name, const char *filename, int buffersize = 8*K, int num_buffer = 8)
        : DiskReader(name.c_str(), filename, buffersize, num_buffer) { }
};


/**
 * A producer reading from /dev/zero
 */
class ZeroReader : public DiskReader {
public:
    ZeroReader(const char *name, int buffersize = 8*K, int num_buffer = 8);
    ZeroReader(std::string &name, int buffersize = 8*K, int num_buffer = 8)
        : ZeroReader(name.c_str(), buffersize, num_buffer) { }

};

/**
 * A producer reading from /dev/random
 */
class RandomReader : public DiskReader {
public:
    RandomReader(const char *name, int buffersize = 8*K, int num_buffer = 8);
    RandomReader(std::string &name, int buffersize = 8*K, int num_buffer = 8)
        : RandomReader(name.c_str(), buffersize, num_buffer) { }
};

/**
 * A consumer, writing data to disk.
 */
class DiskWriter : public Consumer, public Runnable {
public:
    /** create a DiskWriter
     *  @param name         the name of the Consumer
     *  @param filename     the file to write data to
    */
    DiskWriter(const char *name, const char *filename);
    DiskWriter(std::string &name, const char *filename) 
        : DiskWriter(name.c_str(), filename) { }
protected:
    /** the thread runner for a DiskWriter */
    void run();
};

/**
 * A consumer, writing data to /dev/null.
 */
class NullWriter : public DiskWriter {
public:
    /** create a NullWriter */
    NullWriter(const char *name);
    NullWriter(std::string &name) 
        : NullWriter(name.c_str()) { }
};

#endif //__QRND_DISK_H__