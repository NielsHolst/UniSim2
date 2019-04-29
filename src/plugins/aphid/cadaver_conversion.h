#ifndef CADAVERS_CONVERSION_H
#define CADAVERS_CONVERSION_H
#include <base/box.h>

namespace aphid {

class CadaverConversion : public base::Box
{
public:
    CadaverConversion(QString name, QObject *parent);
    void update();
private:
    // Inputs
    QVector<double>
        succumbedApterousNymphs,
        succumbedAlateNymphs,
        succumbedApterousAdult,
        succumbedAlateAdults;
    // Outputs
    double cadavers;
};

}

#endif
