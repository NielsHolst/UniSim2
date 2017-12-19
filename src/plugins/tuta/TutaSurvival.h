#ifndef TUTASURVIVAL_H
#define TUTASURVIVAL_H
#include <base/box.h>

namespace tuta {

class TutaSurvival : public base::Box
{
public: 
    TutaSurvival(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double a0, a1, a2, a3, a4, a5, A, T, scaling;
    // Outputs
    double survival;
};

} //namespace
#endif
