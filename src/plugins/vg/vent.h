/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VENT_H
#define VENT_H

#include <base/box.h>

namespace vg {

class Vent : public base::Box
{
public:
    Vent(QString name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    int number;
    double length, width, state, transmissivity;
    // Outputs
    double area, effectiveArea, maxEffectiveArea;
    // Data
    QString pathTransmissivity;
};

} //namespace

#endif
