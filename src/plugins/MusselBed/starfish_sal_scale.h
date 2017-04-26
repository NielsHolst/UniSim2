/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef STARFISH_SAL_SCALE_H
#define STARFISH_SAL_SCALE_H

#include <base/box.h>

namespace MusselBed {

class StarfishSalScale : public base::Box
{
public:
    StarfishSalScale(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double smean;

    // Outputs
    double value;
};

} //namespace


#endif

