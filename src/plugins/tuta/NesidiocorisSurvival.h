#ifndef NESIDIOCORISSURVIVAL_H
#define NESIDIOCORISSURVIVAL_H
#include <base/box.h>

namespace tuta {

class NesidiocorisSurvival : public base::Box
{
public: 
    NesidiocorisSurvival(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double a0, a1, a2, a3, a4, T;
    // Outputs
    double survival;
};

} //namespace
#endif
