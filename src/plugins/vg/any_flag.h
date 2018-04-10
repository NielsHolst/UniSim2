/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ANY_FLAG_H
#define ANY_FLAG_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace vg {

class AnyFlag : public base::Box
{
public: 
    AnyFlag(QString name, QObject *parent=0);
private:
    bool flag;
};

} //namespace
#endif
