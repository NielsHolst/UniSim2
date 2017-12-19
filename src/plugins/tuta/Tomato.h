#ifndef TOMATO_H
#define TOMATO_H
#include <base/box.h>

namespace tuta {

class Tomato : public base::Box
{
public:
    Tomato(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double pestNumber, growthRate, damageRate;
    // Outputs
    double status;
};

}

#endif
