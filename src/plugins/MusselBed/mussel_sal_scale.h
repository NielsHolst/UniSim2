/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef MUSSEL_SAL_SCALE_H
#define MUSSEL_SAL_SCALE_H
#include <QVector>
#include <base/box.h>

namespace MusselBed {

class MusselSalScale : public base::Box
{
public:
    MusselSalScale(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double a,b,salinity;

    // Outputs
    double value;
    QVector<double> salinities;
};

} //namespace

#endif
