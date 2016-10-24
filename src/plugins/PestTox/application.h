/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <base/box.h>

namespace PestTox {

class application : public base::Box
{
public:
    application(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    int day;
    double ae, ar;

    // Output
    double Doseappl;

    // Data
    int daysPassed;
};

} //namespace


#endif
