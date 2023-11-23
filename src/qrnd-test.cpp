

#include "qrnd-core.h"
#include "qrnd-disk.h"





//Producer_Pipe & operator [](Producer &, int n) {
//
//}

class Histogram : public Filter, public Runnable {
public:
    Histogram(const char *name);

    void flush();
protected:
    void run();
};


int main() {

    RandomReader reader("random reader");
    DiskWriter writer("disk writer", "out.bin");
    StreamSplitter splitter("splitter");
    Histogram histogram("histogram");
    DiskWriter histogramm_file("histogram file", "histogram.bin");

    reader >> splitter >> writer;
    splitter[1] >> histogram >> histogramm_file;

    Timer timer(1000);
    Node::start_all();
    timer.wait();
    histogram.flush();
    Node::stop_all();

    return 1;
}