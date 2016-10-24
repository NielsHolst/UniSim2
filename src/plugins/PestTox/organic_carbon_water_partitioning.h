/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_ORGANICCARBONWATERPARTITIONING_H
#define PESTTOX_ORGANICCARBONWATERPARTITIONING_H

#include <base/box.h>

namespace PestTox {

class organiccarbonwaterpartitioning : public base::Box
{
public:
    organiccarbonwaterpartitioning(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Koc, pH, pKa;

    // Output
    double KocpH;
};

} //namespace


#endif
