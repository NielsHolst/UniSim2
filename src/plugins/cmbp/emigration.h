#ifndef EMIGRATION_H
#define EMIGRATION_H
#include <base/box.h>

namespace cmbp {

class Emigration : public base::Box
{
public:
    Emigration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double e2, e3, N;
    // Output
    double value;
};

}

#endif
