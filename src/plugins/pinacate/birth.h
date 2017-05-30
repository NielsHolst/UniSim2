#ifndef Birth_H
#define Birth_H
#include <base/box.h>

namespace pinacate {

class Birth : public base::Box
{
public:
    Birth(QString name, QObject *parent);\

    void reset();
    void update();
private:
    // Inputs
    int birthday;
    double fecundity;
    int currentday;
    // Outputs
    double value;
};

}

#endif
