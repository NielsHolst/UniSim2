#ifndef LOG10_H
#define LOG10_H
#include <base/box.h>

namespace eq {

class Log10 : public base::Box
{
public:
    Log10(QString name, QObject *parent);
    void reset();
    void update();
protected:
    // input
    double x;
    // output
    double value;
};

} //namespace

#endif
