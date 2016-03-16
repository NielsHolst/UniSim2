/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AVERAGE_H
#define AVERAGE_H
#include <QVector>
#include <base/box.h>

namespace base {
    class Port;
}

namespace vg {

class Average : public base::Box
{
public: 
    Average(QString name, QObject *parent=0);
    // standard methods
    void initialize();
    void update();
private:
    QString inputs;
    double value;
    QVector<base::Port*> ports;
};

} //namespace
#endif
