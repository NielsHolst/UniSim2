/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef POTATO_DEVELOPMENT_BY_HOUR_H
#define POTATO_DEVELOPMENT_BY_HOUR_H
#include "potato_development_base.h"

namespace potato {

class PotatoDevelopmentByHour : public PotatoDevelopmentBase
{
public:
    PotatoDevelopmentByHour(QString name, QObject *parent);
    void reset();
    void update();
private:
    //inputs
    double temp;
};

}

#endif
