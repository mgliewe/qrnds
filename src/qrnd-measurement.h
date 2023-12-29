#ifndef __QRND_MEASUREMENT_H__
#define __QRND_MEASUREMENT_H__

#pragma once

#include "qrnd-core.h"

namespace QRND {

/**
 * A filter, keeping track of Frame throuput
 */
class FrameCounter : public Filter, public Runnable {
private:
    unsigned counter;
protected:
    int buffersize;
    int num_buffer;
    int windowsize;

public:
    /** create a VoidSink */
    FrameCounter(const char *name, int buffersize = 1*K, int num_buffer = 8);
    FrameCounter(std::string &name, int buffersize = 1*K, int num_buffer = 8) 
        : FrameCounter(name.c_str(), buffersize, num_buffer) { }
    virtual ~FrameCounter();

    int get_bandwidth();

protected:
    /** the thread runner for a NoMangler */
    void run();
};

/**
 * A filter, measuring the Amplitude of passing frames
 */
class FrameMeter : public Filter, public Runnable {
private:
    unsigned rms_counter;
    unsigned avg_counter;
    int value_counter;
protected:
    int buffersize;
    int num_buffer;
    int windowsize;
public:
    /** create a VoidSink */
    FrameMeter(const char *name, int buffersize = 1*K, int num_buffer = 8);
    FrameMeter(std::string &name, int buffersize = 1*K, int num_buffer = 8) 
        : FrameMeter(name.c_str(), buffersize, num_buffer) { }
    virtual ~FrameMeter();

    int get_rms();
    int get_avg();
    void set_windowsize(int sz);

protected:
    /** the thread runner */
    virtual void run();
};


/**
 * A filter, measuring the Amplitude of passing frames
 */
class Histogram : public Filter, public Runnable {
protected:
    int num_buffer;
    int windowsize;
public:
    Histogram(const char *name, int num_buffer = 8);
    Histogram(std::string &name, int num_buffer = 8) 
        : Histogram(name.c_str(), num_buffer) { }
    virtual ~Histogram();

protected:
    /** the thread runner */
    virtual void run();
};

} // end namespace

#endif // __QRND_MEASUREMENT_H__
