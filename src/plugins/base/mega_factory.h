#ifndef BASE_MEGA_FACTORY_H
#define BASE_MEGA_FACTORY_H

#include <QMap>
#include <QObject>
#include "exception.h"
#include "general.h"

namespace base {
	
class FactoryPlugIn;

class MegaFactory : public QObject
{
public:
    static void loadPlugins();

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
    QString msg = QString("MegaFactory cannot create object '%1' of class '%2'").arg(objectName).arg(className);
    if (!object)
        ThrowException(msg);
    setClassName(object, className);
    T *specificObject = dynamic_cast<T*>(object);
    if (!object)
        ThrowException(msg);
    return specificObject;
}


}//namespace

#endif
