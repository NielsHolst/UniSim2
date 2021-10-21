/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PHENOLOGY_H
#define PHENOLOGY_H
#include <base/box.h>

namespace coffee {

class Phenology : public base::Box
{
public: 
    Phenology(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double delT, rain, rainDoyHi, Tshade, TMatB, TMatT;
//        sinkPMax, kSinkPMax, SINKP;
    int day, DaysPlnOp;
    QDate date;
    // Outputs
    bool dayFill;
    double DVS, dDVS, rDVS, dDVSMAX,
        sensit, dSensit,
        rainDoy, dayFl;
//    SINKPMAXnew;
};

} //namespace
#endif
