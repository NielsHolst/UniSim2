/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef STARFISH_TEMP_SCALE_H
#define STARFISH_TEMP_SCALE_H

#include <base/box.h>

namespace MusselBed {

class StarfishTempScale : public base::Box
{
public:
    StarfishTempScale(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double temperature;

    // Outputs
    double value;
};

} //namespace


#endif

