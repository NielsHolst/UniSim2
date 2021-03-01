/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HOST_H
#define HOST_H
#include <base/box.h>

namespace boxes {

class Host : public base::Box
{
public: 
    Host(QString name, QObject *parent);
private:
    // Inputs
    double density,
        attackRate, gainFactor,
        numAttacks, numHostAttacked;
};

} //namespace
#endif
