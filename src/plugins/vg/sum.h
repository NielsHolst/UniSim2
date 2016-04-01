/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUM_H
#define SUM_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace vg {

class Sum : public base::Box
{
public: 
    Sum(QString name, QObject *parent=0);
    // standard methods
    void initialize();
    void update();
private:
    QVector<QString> inputs;
    double value;
    QVector<base::Port*> ports;
};

} //namespace
#endif
