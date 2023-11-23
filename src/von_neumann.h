
#ifndef __VON_NEUMANN_H__
#define __VON_NEUMANN_H__

#include <qrnd-cotre.h>

class VonNeumannDebias : public Filter, public Runnable {
public:
    VonNeumannDebias(const char *name);
    ~VonNeumannDebias(const char *name);

protected:
    void run();
};

#endif // __VON_NEUMANN_H__
