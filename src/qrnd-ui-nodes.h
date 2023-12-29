#include "qrnd.h"
#include "qrnd-ui.h"


#ifndef __QRND_UI_NODES_H__
#define __QRND_UI_NODES_H__


/** 
 * A synthetic class, ued in composer
 * 
 * @ui-class        LineDiagram
 * @ui-summary                      A line diagram with 4 inputs.
 * @ui-name         LineDiagram
 * @ui-inputs       4
 * @ui-input                        the first input
 * @ui-input                        the second input
 * @ui-input                        the third input
 * @ui-input                        the fourth input
 * @ui-outputs      0
 * @ui-category     diagram
 * @ui-color        #E6E0F8
 * @ui-icon         arrow-in.png
 */


namespace QRND { namespace UI {


class FrameBuffer : public Node, public QRND::FrameBuffer {
public:
    FrameBuffer(std::string &name)
        : FrameBuffer(name.c_str()) { }
    FrameBuffer(const char *name)
        : UI::Node(name), QRND::FrameBuffer(name) { }
    virtual ~FrameBuffer() { }

    virtual cJSON *get_values() {
        cJSON *json = UI::Node::get_values();
        return json;
    }

    virtual cJSON *get_data() {
        QRND::Frame *frame = QRND::FrameBuffer::get_data();
        cJSON *result = js_array();

        for(int n=0; n<frame->count; ++n) {
            js_push(result, frame->data[n]);
        }

        QRND::FrameBuffer::release_data();
        return result;
    }

};


/** 
 * @ui-class        NullProducer
 * @ui-summary                      A producer sending all Zeros.
 * @ui-description                  This producer is mainly intended for testing purposes.
 * @ui-name         NullProducer
 * @ui-property     name            name ctr imutable
 * @ui-comment                      The class name shown in Composer                      
 * @ui-property     buffersize      number ctr imutable
 * @ui-comment                      Framesize for the output stream
 * @ui-default      32768
 * @ui-property     num_buffer      number ctr imutable
 * @ui-comment                      Number of preallocated Frames
 * @ui-default      16
 * @ui-inputs       0
 * @ui-outputs      1
 * @ui-output                       the output stream
 * @ui-category     producer
 * @ui-color        #E6E0F8
 * @ui-icon         arrow-in.png
 */
class NullProducer : public Node, public QRND::NullProducer {
public:
    NullProducer(std::string &name, int buffersize = 8*K, int num_buffer = 8)
        : NullProducer(name.c_str(), buffersize, num_buffer) { }
    NullProducer(const char *name, int buffersize = 8*K, int num_buffer = 8)
        : UI::Node(name), QRND::NullProducer(name, buffersize, num_buffer) { }
    virtual ~NullProducer() { }

    virtual cJSON *get_values() {
        cJSON *json = UI::Node::get_values();
        js_set(json, "buffersize", buffersize);
        js_set(json, "num_buffer", num_buffer);
        return json;
    }

};

/** 
 * @ui-class        RandProducer
 * @ui-summary                      A producer using libs rand() as its input.
 * @ui-description                  This producer is mainly intended for testing purposes.
 * @ui-name         RandProducer
 * @ui-property     name            name ctr imutable
 * @ui-comment                      The class name shown in Composer                      
 * @ui-property     buffersize      number ctr imutable
 * @ui-default      32768
 * @ui-comment                      Framesize for the output stream
 * @ui-property     num_buffer      number ctr imutable
 * @ui-comment                      Number of preallocated Frames                                         
 * @ui-default      16
 * @ui-property     seed            number
 * @ui-comment                      The seed values used for srand(). Use 0 for calling time()                                   
 * @ui-inputs       0
 * @ui-outputs      1
 * @ui-output                       the output stream
 * @ui-category     producer
 * @ui-color        #E6E0F8
 * @ui-icon         arrow-in.png
 */
class RandProducer : public Node, public QRND::RandProducer {
public:
    RandProducer(std::string &name, int buffersize = 8*K, int num_buffer = 8)
        : RandProducer(name.c_str(), buffersize, num_buffer) { }
    RandProducer(const char *name, int buffersize = 8*K, int num_buffer = 8)
        : UI::Node(name), QRND::RandProducer(name, buffersize, num_buffer) { }
    virtual ~RandProducer() { }

    virtual cJSON *get_values() {
        cJSON *json = UI::Node::get_values();
        js_set(json, "buffersize", buffersize);
        js_set(json, "num_buffer", num_buffer);
        js_set(json, "seed", seed);
        return json;
    }

};


/** 
 * @ui-class        VoidSink
 * @ui-summary                      A consumer discarding al input.
 * @ui-description                  This consumer is mainly intended for testing purposes.
 * @ui-name         VoidSink
 * @ui-property     name            name ctr imutable
 * @ui-comment                      The class name shown in Composer                      
 * @ui-inputs       1
 * @ui-input                         the input stream
 * @ui-outputs      0
 * @ui-category     consumer
 * @ui-color        #E6E0F8
 * @ui-icon         arrow-in.png
 */
class VoidSink : public Node, public QRND::VoidSink {
public:
    VoidSink(std::string &name)
        : VoidSink(name.c_str()) { }
    VoidSink(const char *name)
        : UI::Node(name), QRND::VoidSink(name) { }
    virtual ~VoidSink() { }

};

/** 
 * @ui-class        Stream2Splitter
 * @ui-summary                      A producer splitting its input to several outputs.
 * @ui-description                  The splitter distributes copies of its input to several outputs.
 * @ui-description                  The splitter creates in-memory copies of each input frame. 
 * @ui-name         Stream2Splitter
 * @ui-property     name            name ctr imutable
 * @ui-comment                      The class name shown in Composer                      
 * @ui-property     buffersize      number ctr imutable
 * @ui-comment                      Framesize for the output stream
 * @ui-default      32768
 * @ui-property     num_buffer      number ctr imutable 
 * @ui-comment                      Number of preallocated Frames                                         
 * @ui-default      32
 * @ui-inputs       1
 * @ui-input                        the input stream
 * @ui-outputs      2
 * @ui-output                       the original input stream
 * @ui-output                       a copy of the input stream
 * @ui-category     dataflow
 * @ui-color        #E6E0F8
 * @ui-icon         arrow-in.png
 */
class Stream2Splitter : public Node, public QRND::StreamSplitter {
public:
    Stream2Splitter(std::string &name, int buffersize = 8*K, int num_buffer = 8) 
        : Stream2Splitter(name.c_str(), buffersize, num_buffer) { }
    Stream2Splitter(const char *name, int buffersize = 8*K, int num_buffer = 8) 
        : UI::Node(name), QRND::StreamSplitter(name, buffersize, num_buffer, 4) { }
    virtual ~Stream2Splitter() { }

    virtual cJSON *get_values() {
        cJSON *json = UI::Node::get_values();
        js_set(json, "buffersize", buffersize);
        js_set(json, "num_buffer", num_buffer);
        return json;
    }
};

/** 
 * @ui-class        Stream4Splitter
 * @ui-summary                      A producer splitting its input to several outputs.
 * @ui-description                  The splitter distributes copies of its input to several outputs.
 * @ui-description                  The splitter creates in-memory copies of each input frame. 
 * @ui-name         Stream4Splitter
 * @ui-property     name            name ctr imutable
 * @ui-comment                      The class name shown in Composer                      
 * @ui-property     buffersize      number ctr imutable 
 * @ui-comment                      Framesize for the output stream
 * @ui-default      32768
 * @ui-property     num_buffer      number ctr imutable
 * @ui-comment                      Number of preallocated Frames                                         
 * @ui-default      64
 * @ui-inputs       1
 * @ui-input                        the input stream
 * @ui-outputs      4
 * @ui-output                       the original input stream
 * @ui-output                       first copy of the input stream
 * @ui-output                       second copy of the input stream
 * @ui-output                       third copy of the input stream
 * @ui-category     dataflow
 * @ui-color        #E6E0F8
 * @ui-icon         arrow-in.png
 */
class Stream4Splitter : public Node, public QRND::StreamSplitter {
public:
    Stream4Splitter(std::string &name, int buffersize = 8*K, int num_buffer = 8) 
        : Stream4Splitter(name.c_str(), buffersize, num_buffer) { }
    Stream4Splitter(const char *name, int buffersize = 8*K, int num_buffer = 8) 
        : UI::Node(name), QRND::StreamSplitter(name, buffersize, num_buffer, 4) { }
    virtual ~Stream4Splitter() { }

    virtual cJSON *get_values() {
        cJSON *json = UI::Node::get_values();
        js_set(json, "buffersize", buffersize);
        js_set(json, "num_buffer", num_buffer);
        return json;
    }
};



/** 
 * @ui-class        AmplitudeMeter
 * @ui-summary                      A filter, measuring the rms amplitude of its input.
 * @ui-name         AmplitudeMeter
 * @ui-property     name            name ctr imutable
 * @ui-comment                      The class name shown in Composer
 * @ui-property     buffersize      number ctr imutable 
 * @ui-comment                      Framesize for the output stream
 * @ui-default      1024
 * @ui-property     num_buffer      number ctr imutable
 * @ui-comment                      Number of preallocated Frames                                         
 * @ui-default      8
 * @ui-property     windowsize      number mutable
 * @ui-comment                      Windows size for measurement
 * @ui-default      32768
 * @ui-property     avg             number runtime
 * @ui-comment                      Current average value of the amplitude                       
 * @ui-property     rms             number runtime
 * @ui-comment                      Current mean square root value of the amplitude                       
 * @ui-inputs       1
 * @ui-input                        the input stream
 * @ui-outputs      3
 * @ui-output                       the original input stream
 * @ui-output                       Signal rms amplitude over time
 * @ui-output                       Signal avg amplitude over time
 * @ui-category     measurement
 * @ui-color        #E6E0F8
 * @ui-icon         arrow-in.png
 */
class AmplitudeMeter : public Node, public QRND::FrameMeter {
public:
    AmplitudeMeter(std::string &name, int buffersize = 1*K, int num_buffer = 8) 
        : AmplitudeMeter(name.c_str(), buffersize, num_buffer) { }
    AmplitudeMeter(const char *name, int buffersize = 1*K, int num_buffer = 8) 
        : UI::Node(name), QRND::FrameMeter(name) { }
    virtual ~AmplitudeMeter() { }

    virtual cJSON *get_values() {
        cJSON *json = UI::Node::get_values();
        js_set(json, "windowsize", windowsize);
        js_set(json, "buffersize", buffersize);
        js_set(json, "num_buffer", num_buffer);
        js_set(json, "rms", get_rms());
        js_set(json, "avg", get_avg());
        return json;
    }
};

/** 
 * @ui-class        BandwidthMeter
 * @ui-summary                      A filter, measuring the rms amplitude of its input.
 * @ui-name         BandwidthMeter
 * @ui-property     name            name ctr imutable
 * @ui-comment                      The class name shown in Composer
 * @ui-property     buffersize      number ctr imutable 
 * @ui-comment                      Framesize for the output stream
 * @ui-default      1024
 * @ui-property     num_buffer      number ctr imutable
 * @ui-comment                      Number of preallocated Frames                                         
 * @ui-default      8
 * @ui-property     windowsize      number mutable
 * @ui-comment                      Windows size for measurement
 * @ui-default      32768
 * @ui-property     bandwidth       number runtime
 * @ui-comment                      Current bandwidth                       
 * @ui-inputs       1
 * @ui-input                        the input stream
 * @ui-outputs      2
 * @ui-output                       the original input stream
 * @ui-output                       bandwidth over time
 * @ui-category     measurement
 * @ui-color        #E6E0F8
 * @ui-icon         arrow-in.png
 */
class BandwidthMeter : public Node, public QRND::FrameCounter {
public:
    BandwidthMeter(std::string &name, int buffersize = 1*K, int num_buffer = 8) 
        : BandwidthMeter(name.c_str(), buffersize, num_buffer) { }
    BandwidthMeter(const char *name, int buffersize = 1*K, int num_buffer = 8) 
        : UI::Node(name), QRND::FrameCounter(name, buffersize, num_buffer) { }
    virtual ~BandwidthMeter() { }

    virtual cJSON *get_values() {
        cJSON *json = UI::Node::get_values();
        js_set(json, "buffersize", buffersize);
        js_set(json, "num_buffer", num_buffer);
        js_set(json, "bandwidth", get_bandwidth());
        js_set(json, "windowsize", windowsize);
        return json;
    }
};

/** 
 * @ui-class        Histogram
 * @ui-summary                      A filter, collecting the signal distribution of the input.
 * @ui-name         Histogram
 * @ui-property     name            name ctr imutable
 * @ui-comment                      The class name shown in Composer
 * @ui-property     num_buffer      number ctr imutable
 * @ui-comment                      Number of preallocated Frames                                         
 * @ui-default      4
 * @ui-property     windowsize      number mutable
 * @ui-comment                      Windows size for measurement
 * @ui-default      32768
 * @ui-inputs       1
 * @ui-input                        the input stream
 * @ui-outputs      2
 * @ui-output                       the original input stream
 * @ui-output                       Signal distribution
 * @ui-category     measurement
 * @ui-color        #E6E0F8
 * @ui-icon         arrow-in.png
 */
class Histogram : public Node, public QRND::Histogram {
public:
    Histogram(std::string &name, int num_buffer = 8) 
        : Histogram(name.c_str(), num_buffer) { }
    Histogram(const char *name, int num_buffer = 8) 
        : UI::Node(name), QRND::Histogram(name, num_buffer) { }
    virtual ~Histogram() { }

    virtual cJSON *get_values() {
        cJSON *json = UI::Node::get_values();
        js_set(json, "num_buffer", num_buffer);
        js_set(json, "windowsize", windowsize);
        return json;
    }
};

} } // end namespace

#endif // __QRND_UI_NODES_H__