/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_LEAFOVERALLRATECONSTANT_H
#define PESTTOX_LEAFOVERALLRATECONSTANT_H

#include <base/box.h>

namespace PestTox {

class leafoverallrateconstant : public base::Box
{
public:
    leafoverallrateconstant(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double klu, klv, kld;

    // Output
    double kov;
};

} //namespace


#endif
