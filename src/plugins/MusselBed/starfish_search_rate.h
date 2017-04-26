/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef STARFISH_SEARCH_RATE_H
#define STARFISH_SEARCH_RATE_H

#include <base/box.h>

namespace MusselBed {

class StarfishSearchRate : public base::Box
{
public:
    StarfishSearchRate(QString name, QObject *parent);
    void reset();
    void initialize();
    void update();

private:
    // Inputs
    double s;
    // Outputs
    double value;


    // Links
    QVector<base::Box*> scales;
};

} //namespace

#endif
