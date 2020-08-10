#ifndef LIN_DEV_RATE_H
#define LIN_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class LinDevRate : public base::Box
{
public:
    LinDevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a0, a1;

    // Data
    double lineardevrate;
};

}

#endif
