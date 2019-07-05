/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and Antonio Aguera (antonio.aguera@gmail.com).
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef STARFISH_HD_SCALE_H
#define STARFISH_HD_SCALE_H

#include <base/box.h>

namespace MusselBed {

class StarfishHdScale : public base::Box
{
public:
    StarfishHdScale(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double a, b, vmax, mBiomass;

    // Outputs
    double scale, value;
};

} //namespace


#endif
