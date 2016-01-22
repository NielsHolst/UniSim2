#ifndef PRODUCT_BASE_H
#define PRODUCT_BASE_H

#include <QMap>
#include <QString>

class QObject;

namespace base {

class ProductBase;
typedef QMap<QString, ProductBase*> ProductList;

class ProductBase
{
public:
    ProductBase(QString name, ProductList &list);
    virtual QObject* create(QString objectName, QObject *parent) const = 0;
    QString id() const;
private:
    QString _id;
};

} //namespace

#endif
