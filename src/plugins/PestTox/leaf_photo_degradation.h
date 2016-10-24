/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PESTTOX_LEAFPHOTODEGRADATION_H
#define PESTTOX_LEAFPHOTODEGRADATION_H

#include <base/box.h>

namespace PestTox {

class leafphotodegradation : public base::Box
{
public:
    leafphotodegradation(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double I, kOH, dl;

    // Output
    double concentration, outflow, kld, log_OH;
};

} //namespace


#endif
