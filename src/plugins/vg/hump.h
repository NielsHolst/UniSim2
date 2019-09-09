/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HUMP_H
#define HUMP_H
#include <base/box.h>

namespace vg {

class Hump : public base::Box
{
public: 
    Hump(QString name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();
private:
    // Input
    double x, x0, x1, ymin, ymax;
    bool ditch;
    // Output
    double value;
};

} //namespace
#endif
