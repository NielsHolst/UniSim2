#ifndef CADAVERS_UNITS_H
#define CADAVERS_UNITS_H
#include <base/box.h>

namespace aphid {

class cadavers_units : public base::Box
{
public:
    cadavers_units(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector <double> nb_cadavers_adults;
    QVector <double> nb_cadavers_nymphs;
    //QVector <double> nb_cadavers_alates;
    //QVector <double> nb_cadavers_N_A;
    // Outputs
    QVector<double> nb_units;
    //Data
    QVector<double> result1;
   // QVector<double> result2;
};

}

#endif
