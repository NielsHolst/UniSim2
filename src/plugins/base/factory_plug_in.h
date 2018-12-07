#ifndef BASE_FACTORY_PLUG_IN_H
#define BASE_FACTORY_PLUG_IN_H

#include <QString>
#include <QtPlugin>
#include "exception.h"
#include "product_base.h"

class QObject;

namespace base {

class FactoryPlugIn
{
public:
    virtual QString id() const = 0;
    virtual QList<QString> inventory() const = 0;
    virtual QObject* create(QString className, QString objectName, QObject *parent) = 0;
};

} //namespace

Q_DECLARE_INTERFACE(base::FactoryPlugIn, "org.ecolmod.base.FactoryPlugIn")

#endif
