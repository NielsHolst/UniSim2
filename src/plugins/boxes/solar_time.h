/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SOLAR_TIME_H
#define SOLAR_TIME_H
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <base/box.h>
#include <base/time_with_units.h>

namespace boxes {

class SolarTime : public base::Box
{
public: 
    SolarTime(QString name, QObject *parent);
    void reset();
	void update();
private:
    // inputs
    double latitude, longitude;
    int dayOfYear, timeZone;
    QTime time;
    // outputs
    QTime trueSolarTime;
    double azimuth;
};

} //namespace
#endif
