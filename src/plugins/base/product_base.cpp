#include "product_base.h"

namespace base {

ProductBase::ProductBase(QString id, ProductList &list)
    : _id(id)
{
    list[id] = this;
}

QString ProductBase::id() const {
    return _id;
}

} //namespace
