/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_FACTORY_PLUG_IN_H
#define BASE_FACTORY_PLUG_IN_H

#include <QString>
#include <QtPlugin>
#include "exception.h"
#include "product_base.h"


namespace base {

class Node;

class FactoryPlugIn
{
public:
    virtual QString id() const = 0;
    virtual QList<QString> inventory() const = 0;
    virtual Node* create(QString className, QString objectName, Node *parent) = 0;
};

} //namespace

Q_DECLARE_INTERFACE(base::FactoryPlugIn, "org.ecolmod.base.FactoryPlugIn")

#endif
