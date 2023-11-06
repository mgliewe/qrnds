

/** 
 * 
 * Simple Consumer/Producer pattern using FiFos
 * 
 * 
 */


#include <string>
#include <vector>
#include <map>

#include <pthread.h>
#include <semaphore.h>
#include <math.h>




typedef uint16_t value_type;

// timeout multiplier
const int SEC = 1000000000;
const int MSEC = 1000000;
const int PSEC = 1000;
const int NSEC = 1;

// buffersize multiplier
const int K = 1024;
const int M = K * K;
const int G = M * K;


const int S_RUNNING = 1;
const int S_PAUSED = 2;

const int S_PRODUCER = 0x100;
const int S_CONSUMER = 0x200;
const int S_TIMER = 0x400;

const int S_RECEIVING = 0x10;
const int S_SENDING = 0x20;
const int S_STALLED = 0x30;


class Runnable;
class Lockable;
class Semaphore;

class Frame;
class FrameStream;
class Producer;
class Consumer;

/**
 * A classic Mutex
 */
class Lockable {
    pthread_mutex_t mutex;
public:
    Lockable();
    ~Lockable();
    void lock();
    void unlock();
};


/**
 * A classic counting Semaphore
 */
class Semaphore {
    sem_t sem;
public:
    Semaphore();
    ~Semaphore();
    void wait();
    void wait(long timeout);
    void post();
};


/**
 * A class Thread 
 */
class Runnable {
private:
    pthread_t pid;
    Semaphore sem;

protected:
    int run_state;

public:
    Runnable();
    ~Runnable();

    void set_priority(int priority);

    void start();
    void stop();

    void pause();
    void resume();

    void wait_for();


    inline bool is_stopped() { return !(run_state & S_RUNNING); }
    inline bool is_paused() { return run_state & S_PAUSED; }
    inline bool is_running() { return !( is_paused() || is_stopped() ); }

protected:
    void wait_for_resume();

    virtual void run() = 0;

private:
    static void *thread_proc(void *ctx);
};


/**
 * A Semaphore, that fires periodical.
*/
class Timer : public Runnable, public Semaphore {
private:
    Semaphore sem;
    int period;
public:
    Timer(int period);

protected:
    void run();
};


/**
 * An abstract Task, that is run periodical.
 * 
 * Subclasses have to implement the action() method. 
 */
class Interval : public Runnable {
private:
    Timer tm;
public:
    Interval(int period);
    virtual void action() = 0;

protected:
    void run();
};


/**
 * A set of datapoints.
 * 
 * Buffers are owned by a Producer.
 * 
 * The Producer fills up data and sends it to a Consumer.
 * After processing the data the Consumer gives the Frame back to the
 * owning Producer for reuse.
 * 
 * Buffers are passed between Producer/Consumer via BufferStreams. 
 */
struct Frame {
    int size;
    int capacity;
    value_type *data;
    Producer *owner;        
    Frame *link;                   // used for Fifos and Lists
    Frame *alloc_link;             // used to keep a list of all allocated buffers
    void (*destruct)(void *);     // free() function for data

    Frame(Producer &owner, int capacity, value_type *Frame = 0, void(*destruct)(void *) = 0);
    ~Frame();
    void give_back();
};


/**
 * A classic Fifo, used to pass Buffers between Producer and Consumer.
 * 
 * 
 */
class FrameStream : protected Lockable, protected Semaphore {
    Frame * _first;
    Frame * _last;

public:
    FrameStream();

    void append(Frame *);
    Frame *shift(long timeout);
    Frame *shift() {
        return shift(0);
    }
    Frame *top();
};


/**
 * Usage statics of a Producer/Consumer
 * 
 * Keeps record of Data thruput and bandwidth.
 * 
 * The bandwidth stats will e updated on each tick
 * 
 * @see Node::tick()
 */
struct Statistics {
    long input_bytes_processed;
    long input_bandwidth;

    long ouput_bytes_processed;
    long output_bandwidth;

    Statistics() 
        : input_bytes_processed(0), input_bandwidth(0), 
          ouput_bytes_processed(0), output_bandwidth(0) { }
    void tick() {
        input_bandwidth = input_bytes_processed;
        input_bytes_processed = 0;
        output_bandwidth = ouput_bytes_processed;
        ouput_bytes_processed = 0;
    }
};


/**
 * An abstract Node in the Producer/Consumer graph.
 * 
 * The class keeps a static list of all created Nodes for
 * reporting and debugging uses.
 * 
 * Make sure to initialize the Node class via Node::init() before
 * creating any Producer or Consumer.
 */
class Node {

public:
    typedef void (*iterator_func)(Node &, void *);

    static void init();
    static int for_each( iterator_func, void *ctx);

    static void tick();

private:
    static void add_node(Node *node);
    static void remove_node(Node *node);


public:
    std::string name;
    int state;

    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    Statistics stats;

    Node(const char *name) { add_node(this); }
    ~Node() { remove_node(this); }   // note: this might be called twice for filters
    
    virtual bool is_consumer() { return false; }
    virtual bool is_producer() { return false; }

    virtual void on_tick() { }

};


class Consumer : public Node {
protected:
    friend class Producer;      // allow Producer to access input
    FrameStream input;
public:
    Consumer(const char *name) : Node(name) { }
    Consumer(std::string &name) : Consumer(name.c_str()) { }
    virtual ~Consumer() { Node::~Node(); }

    Frame *receive();

    virtual bool is_consumer() { return true; }
};

/**
 * An abstract data producer node
 * 
 * A producer will output data to 'num_outputs' nodes.
 * 
 */
class Producer : public Node {

private:
    FrameStream free_buffers;
    Frame *allocated_buffer;

protected:
    Consumer **output_slots;

public:
    /// @brief  number of output slots
    int num_outputs;

    /**
     * create a producer
     */
    Producer(const char *name, int num_outputs);
    Producer(std::string &name, int num_outputs) 
        : Producer(name.c_str(), num_outputs) { }
    virtual ~Producer();

    virtual bool is_producer() { return true; }

    /**
     * add a Frame to the internal FrameStream
     */
    void add_frame(Frame *);
      
    /**
     * get next available Frame, stall until a new
     * Frame is available
     */
    Frame *get_frame();

    /**
     * send a data-Frame to a consumer
     * 
     * @param frame         the Frame
     */
    void send(int channel, Frame *frame);

    /**
     * retake ownership of a Frame, making it available for get_buffer()
     * 
     * @param frame         the Frame
     */
    void take_back(Frame *frame);

    /**
     * connect a consumer to an output channel 
     **/    
    void connect_output(int channel, Consumer &);


    void allocate_buffers(int size, int count);
};


/**
 * A producer producing all zeros
 */
class NullReader : public Producer, public Runnable {
protected:
    void run();
public:
    NullReader(const char *name, int buffersize = 8*K, int num_buffer = 8);
    NullReader(std::string &name, int buffersize = 8*K, int num_buffer = 8) 
        : NullReader(name.c_str(), buffersize, num_buffer) { }
};


/**
 * A producer using libc rand()
 */
class RandReader : public Producer, public Runnable {
private:
    unsigned seed;
protected:
    void run();
public:
    RandReader(const char *name, int buffersize = 8*K, int num_buffer = 8);
    RandReader(std::string &name, int buffersize = 8*K, int num_buffer = 8)
        : RandReader(name.c_str(), buffersize, num_buffer) { }
};


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
 * A consumer that does nothing
 */
class NoWriter : public Consumer, public Runnable {
public:
    /** create a NoWriter */
    NoWriter(const char *name);
    NoWriter(std::string &name) 
        : NoWriter(name.c_str()) { }
protected:
    /** the thread runner for a NoMangler */
    void run();
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
class StreamMangler : public Consumer, public Producer, public Runnable {
private:
    int next_output;    ///< next slot to send data to
public:
    /** create a StreamMangler
     *  @param num_output   no. of output streams
    */
    StreamMangler(const char *name, int num_outputs);
    StreamMangler(std::string &name, int num_outputs) 
        : StreamMangler(name.c_str(), num_outputs) { }
protected:
    /** the thread runner for a StreamMangler */
    void run();
};



