/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef HUMP_H
#define HUMP_H
#include <base/box.h>

namespace vg {

class Hump : public base::Box
{
public: 
    Hump(QString name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();
private:
    // Input
    double x, x0, x1, ymin, ymax;
    bool ditch;
    // Output
    double value;
};

} //namespace
#endif
