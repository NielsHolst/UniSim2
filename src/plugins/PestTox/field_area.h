/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_FIELDAREA_H
#define PESTTOX_FIELDAREA_H

#include <base/box.h>

namespace PestTox {

class fieldarea : public base::Box
{
public:
    fieldarea(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double W, L;

    // Output
    double area;
};

} //namespace


#endif
