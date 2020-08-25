#ifndef STRUCTURALDRYWEIGHT_H
#define STRUCTURALDRYWEIGHT_H
#include <base/box.h>

namespace saccharina {

class Structuraldryweight : public base::Box
{
public:
    Structuraldryweight(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double kA, A;
    // Outputs
    double Ws;
};

}

#endif
