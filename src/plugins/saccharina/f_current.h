#ifndef F_CURRENT_H
#define F_CURRENT_H
#include <base/box.h>

namespace saccharina {

class Fcurrent : public base::Box
{
public:
    Fcurrent(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double uMin, U63, U;
    // Outputs
    double value;
};

}

#endif
