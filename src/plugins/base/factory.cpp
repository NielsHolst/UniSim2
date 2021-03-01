/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "factory.h"
#include <base/publish.h>

//#define RETURN_ID(x) #x

using namespace base;

namespace BOXES_PLUGIN_NAME {

QString Factory::id() const {
    return BOXES_PLUGIN_NAME_TEXT;
}

QList<QString> Factory::inventory() const{
    return productList().keys();
}

QObject* Factory::create(QString className, QString objectName, QObject *parent) {
    ProductList::const_iterator p = productList().find(className);
    return (p == productList().end())
            ? 0
            : p.value()->create(objectName, parent);
}

} //namespace
