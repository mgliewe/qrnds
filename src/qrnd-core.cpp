#include "producer.h"

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>


Lockable::Lockable() {
	int status = pthread_mutex_init(&mutex, NULL);
	assert (0 != status );
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
	assert ( 0 != status );
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
    : pid(0), run_state(0)
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
    run_state &= ~S_RUNNING;
}

void Runnable::pause() {
    run_state |= ~S_PAUSED;
}

void Runnable::resume() {
    if (run_state & S_PAUSED) {
        run_state &= ~S_PAUSED;
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
    runner->run_state |= S_RUNNING;
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

Interval::Interval(int period) 
    : tm(period)
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



Buffer::Buffer(Producer &owner, int capacity, value_type *buffer, void(*destruct)(void *)) 
    : size(0), capacity(capacity), data(buffer), 
      owner(&owner), 
      link(0), alloc_link(0),
      destruct(destruct)
{
    if (!data) {
        data = (value_type *) calloc(sizeof(value_type), capacity);
        destruct = free;
    }
}

Buffer::~Buffer() {
    destruct(this->data);
}

void Buffer::give_back() {
    if (owner) {
        owner->take_back(this);
    } else {
        delete this;
    }
}




BufferStream::BufferStream() 
    : _first(0), _last(0)
{
}

void BufferStream::append(Buffer *buffer) {
    assert( buffer!=NULL );
    lock();
    if (_last) {
        _last->link = buffer;
    }
    _last = buffer;
    Semaphore::post();
    unlock();
}


Buffer *BufferStream::shift(long timeout) {
    Buffer *b;

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
    unlock();
    return b;
}

Buffer *BufferStream::top() {
    Buffer *b;
    lock();
    b = _first;
    unlock();
    return b;
}



Buffer *Consumer::receive() {
    state|=S_RECEIVING; 
    Buffer *b=input.shift(); 
    state&=S_RECEIVING; 
    return b;
}



static std::map<std::string, Node *> *_nodes;

/* static */ void Node::init() {
    _nodes = new std::map<std::string, Node *>;
}

/* static */ void Node::add_node(Node *node) {
    if (_nodes->find(node->name) != _nodes->end())
        throw ("on create node: a Consumer/Producer of this name already exists");
    (*_nodes)[node->name] = node;
}

/* static */  void Node::remove_node(Node *node) {
    _nodes->erase(node->name);
}

/* static */ int Node::for_each( iterator_func, void *ctx) {
    int n = 0;
    for (std::map<std::string, Node *>::iterator it = _nodes->begin(); it!=_nodes->end(); ++it) {
        (iterator_func) (it->second, ctx);
        ++n;
    }
    return n;
}

/* static */ void Node::tick() {
    for (std::map<std::string, Node *>::iterator it = _nodes->begin(); it!=_nodes->end(); ++it) {
        (it->second)->stats.tick();
        (it->second)->on_tick();
    }   
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

void Producer::add_buffer(Buffer *buffer) {
    free_buffers.append(buffer);
    buffer->alloc_link = this->allocated_buffer;
    this->allocated_buffer = buffer;
}

Buffer *Producer::get_buffer() {
    state |= S_STALLED;
    Buffer *b = free_buffers.shift();
    state &= ~S_STALLED;
    return b;
}

void Producer::send(int channel, Buffer *buffer) {
    assert( channel>=0 );
    assert( channel<num_outputs );

    if (output_slots[channel]) {
        state |= S_SENDING;
        output_slots[channel]->input.append(buffer);
        state &= ~S_SENDING;
    } else {
        buffer->give_back();
    }
}

void Producer::take_back(Buffer *buffer) {
    this->free_buffers.append(buffer);
}

void Producer::allocate_buffers(int size, int count) {
    for (int n=0; n<count; ++n) {
        Buffer *b = new Buffer(*this,size);
        add_buffer(b);
    }
}



NullReader::NullReader(const char *name, int buffersize, int num_buffer) 
  : Producer(name, 1)
{
    allocate_buffers(buffersize, num_buffer);
}

void NullReader::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;
        Buffer *b = get_buffer();
        send(0, b);
    }
}


RandReader::RandReader(const char *name, int buffersize, int num_buffer) 
  : Producer(name, 1)
{
    allocate_buffers(buffersize, num_buffer);
    seed = time(NULL);
}

void RandReader::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;
        Buffer *b = get_buffer();
        for (int n=0; n<b->capacity; ++n) {
            b->data[n] = rand_r(&seed);
        }
        b->size = b->capacity;
        send(0, b);
    }

}


DiskReader::DiskReader(const char *name, const char *filename, int buffersize, int num_buffer) 
    : Producer(name, 1)
{
    allocate_buffers(buffersize, num_buffer);

    fd = open(filename, O_RDONLY);
    if (fd<0) {
        throw "canoonot open input file for reading";
    }
}

void DiskReader::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;

        Buffer *buffer = get_buffer();
        uint8_t *data = (uint8_t *) buffer->data;
        int sz = buffer->capacity;
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
        buffer->size = data - (uint8_t *)buffer->data;
        send(0, buffer);
    }
}


ZeroReader::ZeroReader(const char *name, int buffersize, int num_buffer) 
    : DiskReader(name, "/dev/zero", buffersize, num_buffer)
{

}


RandomReader::RandomReader(const char *name, int buffersize, int num_buffer) 
    : DiskReader(name, "/dev/random", buffersize, num_buffer)
{

}




StreamMangler::StreamMangler(const char *name, int num_outputs)
    : Consumer(name), Producer(name, num_outputs), next_output(0) 
{ 

}

void StreamMangler::run() {
    for (;;) {
        if (is_paused()) {
            wait_for_resume();
        }
        if (is_stopped())
            break;

        Buffer *buffer = receive();
        
        // find next output and emit 
        int n = next_output;
        do {
            // if slot is connected, emit
            if (output_slots[n]) {
                send(n, buffer);
                break;
            } 
            // advance to next slot
            n++; 
            if (n>num_outputs) {
                n = 0;
            }
            // if we reached next_output, there are no connections; just dissmiss buffer
            if (n==next_output) {
                buffer->give_back();
            }
        } while (n!=next_output);

        // advance to next slot for next iteration
        n = n + 1;
        if (n>num_outputs) {
            n = 0;
        }
        next_output = n;
    }

}

int main() {

}
