#ifndef __QRND_BASE_H__
#define __QRND_BASE_H__
#pragma once

#include "qrnd-core.h"

namespace QRND {


/**
 * A producer producing all zeros
 */
class NullProducer : public Producer, public Runnable {
protected:
    int buffersize;
    int num_buffer;

public:
    NullProducer(const char *name, int buffersize = 8*K, int num_buffer = 8);
    NullProducer(std::string &name, int buffersize = 8*K, int num_buffer = 8) 
        : NullProducer(name.c_str(), buffersize, num_buffer) { }
    virtual ~NullProducer();

protected:
    void run();

};


/**
 * A producer using libc rand()
 */
class RandProducer : public Producer, public Runnable {
protected:
    int buffersize;
    int num_buffer;
    unsigned seed;

    void run();
public:
    RandProducer(const char *name, int buffersize = 8*K, int num_buffer = 8);
    RandProducer(std::string &name, int buffersize = 8*K, int num_buffer = 8)
        : RandProducer(name.c_str(), buffersize, num_buffer) { }
    RandProducer();
    virtual ~RandProducer();

    void set_seed(int seed);
};


/**
 * A consumer that does nothing
 */
class VoidSink : public Consumer, public Runnable {
public:
    /** create a VoidSink */
    VoidSink(const char *name);
    VoidSink(std::string &name) 
        : VoidSink(name.c_str()) { }
    virtual ~VoidSink();
protected:
    /** the thread runner */
    void run();
};



/**
 * A consumer, buffering its most recent input frame
 */
class FrameBuffer : public Consumer, public Runnable {
    Frame *last_frame;
    Lockable data_lock;
    
public:
    /** create a VoidSink */
    FrameBuffer(const char *name);
    FrameBuffer(std::string &name) 
        : FrameBuffer(name.c_str()) { }
    virtual ~FrameBuffer();
protected:
    /** the thread runner */
    void run();

    Frame *get_data();
    void release_data();
};


/**
 * A filter, dropping incoming frames if the output side is busy
 */
class LazyForward : public Filter, public Runnable {

protected:
    int buffersize;
    int num_buffer;

public:
    /** create a VoidSink */
    LazyForward(const char *name, int buffersize = 8*K, int num_buffer = 8);
    LazyForward(std::string &name, int buffersize = 8*K, int num_buffer = 8) 
        : LazyForward(name.c_str(), buffersize, num_buffer) { }
    virtual ~LazyForward();

protected:
    /** the thread runner */
    void run();
};


class StreamSplitter : public Filter, public Runnable {
protected:
    int buffersize;
    int num_buffer;

public:
    StreamSplitter(const char *name, int buffersize = 8*K, int num_buffer = 8, int num_outputs = 2);
    StreamSplitter(std::string &name, int buffersize = 8*K, int num_buffer = 8, int num_outputs = 2)
        : StreamSplitter(name.c_str(), buffersize, num_buffer, num_outputs) 
        { } 
    virtual ~StreamSplitter();

protected:
    /** the thread runner */
    void run();
};

/**
 * A filter, distributing its input to several outputs
 * 
 * It reads from its input and evenly distributes the FrameStream to its
 * outputs. 
 * 
 * Those output consumers thereafter may stream to a single stream by themself.
 * 
 * Main intention is to distribute workload over several execution threads.
 * 
 * Note that when doing so, there is no guaranty of the order, the  
 * processed buffers are send to the resulting output stream.
 * 
 */
class StreamBalancer : public Consumer, public Producer, public Runnable {
private:
    int next_output;    ///< next slot to send data to
public:
    /** create a StreamMangler
     *  @param num_output   no. of output streams
    */
    StreamBalancer(const char *name, int num_outputs);
    StreamBalancer(std::string &name, int num_outputs) 
        : StreamBalancer(name.c_str(), num_outputs) { }
    virtual ~StreamBalancer();
protected:
    /** the thread runner for a StreamMangler */
    void run();
};

} // end namespace

#endif // __QRND_BASE_H__
