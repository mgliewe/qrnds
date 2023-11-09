

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
    /** create a lockable object */
    Lockable();
    /** release a lockable object*/
    ~Lockable();
    /** aquire lock */
    void lock();
    /** release lock */
    void unlock();
};


/**
 * A classic counting Semaphore
 */
class Semaphore {
    sem_t sem;
public:
    /** create a signaling object  */
    Semaphore();
    /** release a signaling object */
    ~Semaphore();
    /** wait for signal */
    void wait();
    /** wait for signal, abrt after timeout naosecs */
    void wait(long timeout);
    /** emit a signal */
    void post();
};


/**
 * An Task, implemented in an execution thread 
 */
class Runnable {
private:
    pthread_t pid;
    Semaphore sem;

protected:
    enum {
        S_RUNNING = 1,
        S_PAUSED = 2
    } run_state;

    enum task_priorities {
        P_HIGH_PRIORITY,
        P_NORMAL_PRIORITY,
        P_LOW_PRIORITY
    };

public:
    /** create a runnable object */
    Runnable();
    /** release a runnable object */
    ~Runnable();

    /** set task priority; must be set prior to start()
     * @param priority  
    */
    void set_priority(int priority);

    /** start the task */
    void start();
    /** stop the task at next convinient time */
    void stop();
    /** wait for task to finish */
    void wait_for();

    /** pause the task at next convinient time */
    void pause();
    /** resume the task when paused */
    void resume();
    /** wait for task to resume */
    void wait_for_resume();

    /** check if task is stopped */
    inline bool is_stopped() { return !(run_state & S_RUNNING); }
    /** check if task is paused */
    inline bool is_paused() { return run_state & S_PAUSED; }
    /** check if task is running */
    inline bool is_running() { return !( is_paused() || is_stopped() ); }

protected:
    /**
     * perform the tasks work.
     * 
     * subclasses must implement this method. 
     */
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
    /** 
     * create a Timer 
     * @param period    interval the timer is signaling
    */
    Timer(int period);

protected:
    /** the timers implementation */
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
    void *ctx;
public:
    Interval(int period, void ctx);
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

protected:
    Producer *owner;        
    Frame *link;                   // used for Fifos and Lists
    Frame *alloc_link;             // used to keep a list of all allocated buffers
    void (*destruct)(void *);      // free() function for data

public:
    int count;
    int capacity;
    value_type *data;

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

    void enqueue(Frame *);
    Frame *dequeue(long timeout);
    Frame *dequeue() {
        return dequeue(0);
    }
    Frame *top();
};


const int HS_BIT = 1 << (8*sizeof(value_type)-1);


/**
 * A bitstream inside a frame.
 * 
*/
class Bitstream {
    Frame *frame;
    value_type *pout;

    value_type data;
    int bit_count;
    value_type data_mask;


public:
    /**
     * construct a bitstream
     * @param out       the Frame to write the bitstream to
     */
    inline Bitstream(Frame *out) 
        : frame(out),
        data(0), bit_count(0) 
    {
        pout = frame.data;
        data_mask = HS_BIT;
    }

    /**
     * Send a bit to the stream.
     * 
     * @param bit       the bit to send
     * @return          if true, the frames capacity is reached
     */
    bool emit(int bit) {
        if (bit)
            data |= bitmask;
        data_mask >>= 1;
        bit_count++;

        if (bit_count==8) {
            *pout++ = data;
            data = 0;
            data_mask = HS_BIT;
            frame->count++;
            if (frame->count==frame->capacity) {
                return true;
            }
        }
        return false;
    }

    /**
     * receive a bit from the strem.
     * 
     * @param bit       the variable the bit is received to
     * @return          if true, the frames capacity is exhausted
    */
    bool receive(int &bit);

    /**
     * rewind to the beginning of the frame
    */
    void rewind() { 
        data = 0;
        data_mask = HS_BIT;
        pout = frame->data;
    }

    /**
     * set a new frame to send to/receive from bits
     */
    inline void set_frame(Frame *out) {
        frame = out;
        pout = frame->data;
    }
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



