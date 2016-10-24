/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_PRIMARYDISTRIBUTIONDRIFTTOAIR_H
#define PESTTOX_PRIMARYDISTRIBUTIONDRIFTTOAIR_H

#include <base/box.h>

namespace PestTox {

class primarydistributiondrifttoair : public base::Box
{
public:
    primarydistributiondrifttoair(QString name, QObject *parent);
    void reset();
    void update();

//Iin shared functions i.e. functions which shares functions, "private" is replaced by "protected".

protected:

    // Input
    double Doseappl, Pnsz, Nd, A0, A1, B0, B1, X0, Y0,W;

    // Output
    double fXpass, Xpass, Dosedrift, fd;

    int npass;
};

} //namespace


#endif
