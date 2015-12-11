#include "product_base.h"

namespace boxes {

ProductBase::ProductBase(QString id, ProductList &list)
    : _id(id)
{
    ProductList test = list;
    list[id] = this;
}

QString ProductBase::id() const {
    return _id;
}

} //namespace
