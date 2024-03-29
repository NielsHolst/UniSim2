/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FACTORY_H
#define FACTORY_H

#include <base/factory_plug_in.h>

namespace BOXES_PLUGIN_NAME {

class Factory : public QObject, public base::FactoryPlugIn
{
Q_OBJECT
Q_INTERFACES(base::FactoryPlugIn)
Q_PLUGIN_METADATA(IID "org.ecolmod.base")
public:
    QString id() const;
    QList<QString> inventory() const;
    QObject* create(QString className, QString objectName, QObject *parent);
};

} //namespace

#endif
