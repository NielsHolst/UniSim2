#include "factory.h"
#include <base/publish.h>

#define RETURN_ID(x) #x

using namespace base;

namespace BOXES_PLUGIN_NAME {

QString Factory::id() const {
    return RETURN_ID(BOXES_PLUGIN_NAME);
}

QList<QString> Factory::inventory() {
    return productList().keys();
}

QObject* Factory::create(QString className, QString objectName, QObject *parent) {
    ProductList::const_iterator p = productList().find(className);
    return (p == productList().end())
            ? 0
            : p.value()->create(objectName, parent);
}

} //namespace
