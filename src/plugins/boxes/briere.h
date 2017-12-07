#ifndef BRIERE
#define BRIERE
#include <base/physiological_time.h>

namespace boxes {

class Briere : public base::PhysiologicalTime
{
public: 
    Briere(QString name, QObject *parent);
    void reset();
    void updateStep();
private:
    // Inputs
    double T0, Tmax, a;
};

} //namespace
#endif
