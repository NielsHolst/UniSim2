#ifndef MEGA_FACTORY_H
#define MEGA_FACTORY_H

#include <QMap>
#include <QObject>
#include "exception.h"
#include "general.h"

namespace base {
	
class FactoryPlugIn;

class MegaFactory : public QObject
{
public:
    template <class T>
    static T* create(QString className, QString objectName, QObject *parent);

    static const QList<base::FactoryPlugIn*> &factories();

private:
    // methods
    MegaFactory();
    static MegaFactory* me();
    static QObject* createObject(QString className, QString objectName, QObject *parent=0);
    // data
    typedef QMap<QString, FactoryPlugIn*> ProductIndex;
    static MegaFactory *_me;
    ProductIndex productIndex;
    QList<base::FactoryPlugIn*> _factories;
    static int productCount;
};

template <class T>
T* MegaFactory::create(QString className, QString objectName, QObject *parent)
{
    QObject *object = MegaFactory::createObject(className, objectName, parent);
    QString msg = "MegaFactory cannot create object '%1' of class '%2'";
    if (!object) {
        throw Exception(msg.arg(objectName).arg(className));
    }
    setClassName(object, className);
    T *specificObject = dynamic_cast<T*>(object);
    if (!specificObject) {
        QString msg2 = msg + ", because '%1' was not derived from class '%2'";
        throw Exception(msg2.arg(className).arg(object->metaObject()->className()));
    }
    return specificObject;
}


}//namespace

#endif
