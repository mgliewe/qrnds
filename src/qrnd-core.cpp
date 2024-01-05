#include "qrnd-core.h"

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <iostream>

namespace QRND {

unsigned long get_time() {
    struct timespec spec;
    unsigned long tm;

    clock_gettime(CLOCK_REALTIME, &spec);

    tm = spec.tv_sec * 1000;
    tm += spec.tv_nsec / 1000000;

    return tm;
}

Lockable::Lockable() {
	int status = pthread_mutex_init(&mutex, NULL);
	assert (0 == status );
}

Lockable::~Lockable() {
    pthread_mutex_destroy(&mutex);
}

void Lockable::lock() {
    pthread_mutex_lock(&mutex);
}

void Lockable::unlock() {
    pthread_mutex_unlock(&mutex);
}


Semaphore::Semaphore() {
    int status = sem_init(&sem, 0, 0);
	assert ( 0 == status );
}

Semaphore::~Semaphore() {
    sem_destroy(&sem);
}

void Semaphore::wait() {
    sem_wait(&sem);
}

void Semaphore::wait(long timeout) {
    if (timeout>=0) {
        struct timespec tm;
        tm.tv_sec = timeout / NSEC;
        tm.tv_nsec = timeout % NSEC;

    } else
        sem_wait(&sem);
}

void Semaphore::post() {
    sem_post(&sem);
}



Runnable::Runnable() 
    : pid(0), run_state(S_STOPPED)
{
}

Runnable::~Runnable() {
    if (pid) {
        pthread_cancel(pid);
        pid = 0;
    }
}

void Runnable::start() {
    pthread_create(&pid, NULL, Runnable::thread_proc, this);
}

void Runnable::stop() {
    run_state = (enum Runnable::run_state) (run_state & ~S_RUNNING);
}

void Runnable::pause() {
    run_state = (enum Runnable::run_state) (run_state | S_PAUSED);
}

void Runnable::resume() {
    if (run_state & S_PAUSED) {
        run_state = (enum Runnable::run_state) (run_state & ~S_PAUSED);
        sem.post();
    }
}

void Runnable::wait_for_resume() {
    while (run_state&S_PAUSED && run_state&S_RUNNING) {
        sem.wait();
    }
}

void Runnable::wait_for() {
    while (run_state& S_RUNNING) {
        sem.wait();
    }
}

/** static **/ void *Runnable::thread_proc(void *ctx) {
    Runnable *runner = static_cast<Runnable *>(ctx);
    runner->run_state = (enum Runnable::run_state) (runner->run_state | S_RUNNING);
    runner->run();
    runner->sem.post();
    return 0;
}



Timer::Timer(int period) :
    period(period) 
{

}

void Timer::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;

        sem.wait(period);

        post();
    }
}

Interval::Interval(int period, void *ctx) 
    : tm(period), ctx(ctx)
{

}

void Interval::run() {
    tm.start();
    for (;;) {
        if (is_paused()) {
            tm.pause();
            wait_for_resume();
            tm.resume();
        }
        if (is_stopped())
            break;
        tm.wait();
        action();
    }
}



Frame::Frame(Producer &owner, int capacity, value_type *frame, void(*destruct)(void *)) 
    : count(0), capacity(capacity), data(frame), 
      owner(&owner), 
      link(0), alloc_link(0),
      destruct(destruct)
{
    if (!data) {
        data = (value_type *) calloc(sizeof(value_type), capacity);
        destruct = free;
    }
}

Frame::~Frame() {
    destruct(this->data);
}

void Frame::give_back() {
    if (owner) {
        owner->take_back(this);
    } else {
        delete this;
    }
}




FrameStream::FrameStream() 
    : Lockable(), Semaphore(), _first(0), _last(0)
{
}

FrameStream::~FrameStream() {

}

void FrameStream::enqueue(Frame *frame) {
    assert( frame!=NULL );
    lock();
    if (_last) {
        _last->link = frame;
    }
    _last = frame;
    if (!_first)    
        _first = frame;
    Semaphore::post();
    unlock();
}


Frame *FrameStream::dequeue(long timeout) {
    Frame *b;

    if (timeout>0) {
        Semaphore::wait(timeout);
    } else {
        Semaphore::wait();
    }

    lock();
    b = _first;
    if (b) {
        _first = b->link;
        b->link = 0;
    }
    if (b==_last) {
        _last = 0;
    }
    unlock();
    return b;
}

/*
Frame *FrameStream::top() {
    Frame *b;
    lock();
    b = _first;
    unlock();
    return b;
}
*/


Frame *Consumer::receive() {
    state|=S_RECEIVING; 
    Frame *b=input.dequeue(); 
    state&=S_RECEIVING; 
    return b;
}

Frame *Consumer::receive_when_available() {
    Frame *b=input.dequeue(1); 
    return b;
}


static std::map<std::string, Node *> _nodes;

/* static */ void Node::init() {
    //_nodes = new std::map<std::string, Node *>;
}

/* static */ void Node::add_node(Node *node) {
    // if (_nodes.find(node->name) != _nodes.end())
    //    throw ("on create node: a Consumer/Producer of this name already exists");
    _nodes[node->name] = node;
}

/* static */  void Node::remove_node(Node *node) {
    _nodes.erase(node->name);
}

/* static */ int Node::for_each( iterator_func, void *ctx) {
    int n = 0;
    for (std::map<std::string, Node *>::iterator it = _nodes.begin(); it!=_nodes.end(); ++it) {
        (iterator_func) (it->second, ctx);
        ++n;
    }
    return n;
}

/* static */ void Node::tick() {
    for (std::map<std::string, Node *>::iterator it = _nodes.begin(); it!=_nodes.end(); ++it) {
        (it->second)->on_tick();
    }   
}

void abort(std::string &msg) {
    std::cerr << msg << std::endl;
    ::abort();
}

void abort(const char *msg) {
    std::string str = msg;
    abort(str);
}

Producer::Producer(const char *name, int num_outputs) 
    : Node(name),
      num_outputs(num_outputs), output_slots(0), allocated_buffer(0)
{
    output_slots = new Consumer *[num_outputs];
}

Producer::~Producer() {
    //TODO: remove producer reference from all owned buffers
    //TODO: release all buffers in free_list
    Node::~Node();
}

void Producer::add_frame(Frame *frame) {
    assert(frame!=0);
    free_buffers.enqueue(frame);
    frame->alloc_link = this->allocated_buffer;
    this->allocated_buffer = frame;
}

Frame *Producer::get_frame() {
    state |= S_STALLED;
    Frame *b = free_buffers.dequeue();
    if (b)
        b->count = 0;
    state &= ~S_STALLED;
    return b;
}

Frame *Producer::get_frame_if_available() {
    Frame *b = free_buffers.dequeue(1);
    return b;
}

void Producer::send(int channel, Frame *frame) {

    // std::cerr << "C:" << channel << " N:" << num_outputs << std::endl;
    assert( channel>=0 );
    assert( channel<num_outputs );

    if (output_slots[channel]) {
        state |= S_SENDING;
        output_slots[channel]->input.enqueue(frame);
        state &= ~S_SENDING;
    } else {
        frame->give_back();
    }
}

void Producer::take_back(Frame *frame) {
    this->free_buffers.enqueue(frame);
}

void Producer::allocate_buffers(int size, int count) {
    for (int n=0; n<count; ++n) {
        Frame *b = new Frame(*this,size);
        add_frame(b);
    }
}

void Producer::release_buffers() {
    // TODO:
}

void Producer::connect_output(int slot, Consumer &c) {
    assert( slot>=0 );
    assert( slot<num_outputs );

    output_slots[slot] = &c;
}

Filter::Filter(const char *name, int num_outputs)
    : Consumer(name), Producer(name, num_outputs)
{

}

Filter::~Filter() {

}

} // end namespace
