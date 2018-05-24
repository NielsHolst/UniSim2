#ifndef CADAVERS_UNITS2_H
#define CADAVERS_UNITS2_H
#include <base/box.h>

namespace aphid {

class cadavers_units2 : public base::Box
{
public:
    cadavers_units2(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector <double> nb_cadavers_adults;
    QVector <double> nb_cadavers_nymphs;
    QVector <double> nb_cadavers_W_nymphs;
    QVector <double> nb_cadavers_alates;
    //QVector <double> nb_cadavers_N_A;
    // Outputs
    QVector<double> nb_units;
    //Data
    QVector<double> resultN;
    QVector<double> resultW;
    QVector<double> resultN_tot;
    QVector<double> result_adult;
};

}

#endif
