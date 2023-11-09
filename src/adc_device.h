

class AdcDevice {
public:
    AdcDevice();
    virtual ~AdcDevice();

    virtual void start_aquisition() = 0;
    virtual int read_frame(Frame *frame) = 0;
    virtual void stop_aquisition() = 0;
};