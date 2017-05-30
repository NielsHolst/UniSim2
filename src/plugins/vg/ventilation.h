/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VENTILATION_H
#define VENTILATION_H

#include <base/box.h>

namespace vg {

class Ventilation : public base::Box
{
public:
    Ventilation(QString name, QObject *parent);
    void amend();
    void reset();

private:
    // Inputs
    double groundArea;
    // Outputs
    double totalLength, averageHeight, proportionalEffectiveArea;
};

} //namespace

#endif
