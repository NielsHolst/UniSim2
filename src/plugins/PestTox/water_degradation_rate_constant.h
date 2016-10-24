/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_WATERDEGRADATIONRATECONSTANT_H
#define PESTTOX_WATERDEGRADATIONRATECONSTANT_H

#include <base/box.h>

namespace PestTox {

class waterdegradationrateconstant : public base::Box
{
public:
    waterdegradationrateconstant(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double DT50;

    // Output
    double k;
};

} //namespace


#endif
