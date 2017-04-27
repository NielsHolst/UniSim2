/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_MACROPOREFLOW_H
#define PESTTOX_MACROPOREFLOW_H

#include <base/box.h>

namespace PestTox {

class MacroporeFlow : public base::Box
{
public:
    MacroporeFlow(QString name, QObject *parent);
    void update();

private:
    // Input
    double fsand, fsilt, fclay, fw, fa, fmacropore, Flowrate,
    frr, tpevent, P, frsw, Tf;

    // Output
    double fporemobile, Vporeimmobile, C, Cused, Cfree,
    Pcapacitymm, Preqh, Preqday, Pi,
    fmp, Vporemobile;
};

} //namespace


#endif
