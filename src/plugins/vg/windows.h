/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_WINDOWS_H
#define VG_WINDOWS_H

#include <base/box.h>

namespace vg {

class Windows : public base::Box
{
public:
    Windows(QString name, QObject *parent);
    void reset();

private:
    // Inputs
    double groundArea, relativeArea, length, height, porosity;
    // Outputs
    double number, effectiveArea;
};

} //namespace

#endif
