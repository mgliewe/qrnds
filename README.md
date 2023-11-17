
# DRAFT - DO NOT USE - WORK IN PROGRESS

# QRNDS - A Framework for Randomness Extraction from Arbitrary Input Sources

## Purpose

The framework is mainly targed to development and lab uses. Its ment as a playground for experiments.

Productional usecases would need a more narrow approach and implementation, giving room for deeper optimizations.

## Constraints

- lightweight
  
  core framework has just a couple 100 of source-lines 

- low dependencies

  core framework works entirely on OS systemcalls and pthreads

- event-based asynchronious operation

  mutex/semaphore synchronization in userspace

- portable

  pure C++11 implementation, no templating

- easy to use

  easy-to-read and easy-to-write code

- multi-threaded

  multi-threaded processing to take advantage of SMP

- high performance

  just pointers get past around, no exhausting memory-to-memory copying. Variable framebuffer sizes, so increasing frame size will scale up performance

## Consumer/Producer Graphs

Based on a a Producer/Consumer pattern.

We construct a graph, where each node is running independantly in its own execution thread.

We distinguesh between producers (generating data frames),  consumers (receiving data frames) and filters (doing both at the same time).

Each consumer has exactly one input, each producer may have 1..n outputs.

Data frames are passed between producer and consumer via frame-streams. 

A frame stream works in a first-in-first-out matter, beeing guarded by mutexes and semaphores - reading from a stream will block until data is available, whereas writing to a stream always successes.

A producer has a finite number of frame-buffers assigned to, which it uses to pass data. Those buffers are assigned to a producer at creation time, but might get assigned more at runtime.

A producer uses those assigned buffers to pass data to its outputs; when a consumer has processed the frame, its buffer is given back to the producer for reuse.

An underflow of the internal - non-used - framebuffer-list will stall a producer, until one of the already-send frames is given back to te producer. 

A filter implements both patterns at the same time, receiving and giving back frames from a producer, and sending and taking back frame buffers from attached consumers.

Filters may decide to drop frames, when its internal buffer-queue is empty. 

## Producer- / Consumer- / Filter-Modules

List of implemented consumer modules in core:

- Nullreader:
  a producer, sending all zero-filled frames

- DiskReader:
  a producer, reading data from disk

- ZeroReader:
  a producer, reading data from /dev/zero, used for benchmarking

- RandomReader:
  a producer, reading data from /dev/random

List of implemented consumer modules in core:

- NoWriter:
  a consumer, dropping all input

- DiskWriter:
  a consumer, writing data to disk

- NullWriter:
  a consumer, writing to /dev/null, used for benchmarking 

- FrameWriter:
  write most recent frame to disk 

List of implemented filter modules in core:

- StreamMangler:
  a filter, distributing an input stream evenly to several consumers

- LazyForward:
  a filter, always accepting input, and passing frames to a consumer, dropping frames on buffer underrun 

- StreamFork:
  a filter, passing frames to several consumer; this is a costly filter, as it involves copying frames into new buffers

Hardware-related Modules:

- AdcDevice:
  abstraction of an attached ADC hardware 

- AdcReader:
  a producer, reading samples from attached hardware
 
Data-processing Filters:

- Histogramm:
  a filter, building a histogram over time

- AutoCorrelate:
  a filter, trying to decorrelate input data based on statistical propabillity
  ?? maybe using the algorithm describred in: https://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=4310156

- VonNeumannDebias:
  a filter, implementing the von Neuman de-bias algorithm https://en.wikipedia.org/wiki/Randomness_extractor#Von_Neumann_extractor

- PeresDebias:
  a filter, implementing the Peres de-bias algorith, sample implementation at https://peteroupc.github.io/randextract.html

- MorinaDebias: (would that be helpfull??)
  a filter, implementing the algorithm as described in https://arxiv.org/abs/1912.09229

- QuantisRandomExtractor:
  a filter, implementing a random extraction algorithm as used in Quantis devices, described in [...]


## Language Bindings

Though written in C++, language binding for python are provided.

Cython [https://cython.org/] is used to implement the bindings, allowing for seemingly integration of up-to-date upstream c++ code into the python bindings.

## Hard- & Software Support

Main development target is linux, using a debian-ish distribution. Ports to other OS, mainly MS-Windows, are feasable, but not planned for the near future.

A Hardware abstraction layer tries to de-couple hardware specifics from the core implementation.

## Examples

### copy a disk file

``` C++

// build graph
Node reader = new DiskReader("text.txt");
Node writer = new DiskWriter("copy-of-text.txt");
reader->connect_output(0, writer);

// perform action
reader->start(); writer->start();
reader->wait_for();
writer->stop();

// cleanup
delete reader; delete writer;
```

### the same, dont use C-pointer

``` C++
// build graph
DiskReader reader("text.txt");
DiskWriter writer("copy-of-text.txt");
reader >> writer;

// perform action
reader.start(); writer.start();
reader.wait_for();
writer.stop();

// no cleanup needed, C-Scope will take care
```

### a simple filter

``` C++
// build graph
DiskReader reader("text.txt");
UpperCaseFilter f("uppercase")
DiskWriter writer("uppercase-of-text.txt");
reader >> f >> writer;

// perform action
Node::start_all();
reader.wait_for();
Node::stop_all();

// no cleanup needed, C-Scope will take care
```

### read from capture card, write raw data to disk

``` C++
// initialize the capture card
AdcDevice dev = new ATS9360("Adc", {
    .wordsize =  ATS9360.EIGHT_BIT_UNSIGNED
});

// create graph
Node *adc = new AdcCapture("Capture")
Node *writer = new DiskWriter("Write to disk", "output.bin");
adc->connect_output(0, writer);

// run for 10 sec
Timer tm = Timer(10 * SEC);
adc->start(); writer->start();
tm.start();
tm.wait();
adc->stop(); writer->stop();
tm.stop();

// show throughput
Node::show_runtime_statistics();

// cleanup
delete adc; delete writer; 
delete dev;

```

### the same, dont use C-pointer

``` C++
// initialize the capture card
AdcDevice dev = ATS9360("Adc", {
    .wordsize =  ATS9360.EIGHT_BIT_UNSIGNED
});

// create graph
AdcCapture adc("Capture", dev)
DiskWriter writer("Write to disk", "output.bin");
adc >> writer;

// run for 10 sec
Timer tm(10 * SEC);
adc.start(); writer.start();
tm.start();
tm.wait();
adc.stop(); writer.stop();
tm.stop();

// show throughput
Node::show_runtime_statistics();

// no cleanup needed, C-Scope will take care

```

### the same in python
``` python
import * from QRNDS

# initialize the capture card
dev = new ATS9360("Adc", {wordsize=EIGHT_BIT_UNSIGNED})

# create graph
adc = new AdcCapture("Capture")
writer = new DiskWriter("Write to disk", "output.bin")
adc.connect_output(0, writer)

# run for 10 sec
tm = new Timer(10 * SEC)
adc.start(); writer.start()
tm.start()
tm.wait()
adc.stop(); writer.stop()
tm.stop()

# show throughput
Node.show_runtime_statistics()

# no cleanup needed, python gc will take care

```

### a more complex setup in C++

``` C++
// initialize the capture card
AdcDevice dev = ATS9360("Adc", {
    .wordsize =  ATS9360.EIGHT_BIT_UNSIGNED
});

// create graph
AdcCapture adc("Capture", dev);
VonNeumanDebias debias("Debias");
Histogram histogram("Histogram");
StreamFork fork("1-to-2 switch", 2);
DiskWriter writer("Write to disk", "output.bin");
FrameWriter hwriter("Write Hidtogram", "histogram.bin");

adc >> debias;
debias >> fork;
fork[0] >> histogram
fork[1] >> writer;
histogram >> hwriter;

// run for 10 sec
Timer tm(10 * SEC);
tm.start();
Node::start_all();
tm.wait();
Node::stop_all();
tm.stop();

// show throughput
Node::show_runtime_statistics();

// no cleanup needed, C-Scope will take care

```

### the same setup in C++, using multi-threading

``` C++
// initialize the capture card
AdcDevice dev = ATS9360("Adc", {
    .wordsize =  ATS9360.EIGHT_BIT_UNSIGNED
});

// create graph
AdcCapture adc("Capture", dev);
StreamMangler dist("distributer", 4);
RandomExtractor re1("Randon Extractor #1");
RandomExtractor re2("Randon Extractor #1");
RandomExtractor re3("Randon Extractor #1");
RandomExtractor re4("Randon Extractor #1");
StreamFork fork("1-to-2 switch", 2)
Histogram histogram("Histogram");
DiskWriter writer("Write to disk", "output.bin");
FrameWriter hwriter("Write Hidtogram", "histogram.bin");

adc >> dist;
dist[0]>>re1;
re1 >> fork;
dist[1]>>re2;
re2 >> fork;
dist[2]>>re3;
re3 >> fork;
dist[3]>>re4;
re4 >> fork;
fork[0] >> histogram
fork[1] >> writer;
histogram >> hwriter;

// run for 10 sec
Timer tm(10 * SEC);
tm.start();
Node::start_all();
tm.wait();
Node::stop_all();
tm.stop();

// show throughput
Node::show_runtime_statistics();

// no cleanup needed, C-Scope will take care

```
