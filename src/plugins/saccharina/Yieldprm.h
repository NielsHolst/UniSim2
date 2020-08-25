#ifndef YIELDPRM_H
#define YIELDPRM_H
#include <base/box.h>

namespace saccharina {

class Yieldprm : public base::Box
{
public:
    Yieldprm(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Iden, Ww;
    // Outputs
    double Y;
};

}

#endif
