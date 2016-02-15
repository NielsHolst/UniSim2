/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTS_H
#define VG_VENTS_H

#include <base/box.h>

namespace vg {

class Vents : public base::Box
{
public:
    Vents(QString name, QObject *parent);
    void reset();

private:
    // Inputs
    double groundArea;
    // Outputs
    double totalLength, averageHeight, proportionalEffectiveArea;
};

} //namespace

#endif
