/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PRODUCT_BASE_H
#define BASE_PRODUCT_BASE_H

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
