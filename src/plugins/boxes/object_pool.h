#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <memory>
#include <QList>
#include <QMap>
#include <QObject>
#include "exception.h"

namespace boxes {
	
class ObjectPool : public QObject
{
public:
    void attach(QString id, QObject *object);
    bool contains(QString id);
    template <class T> T find(QString);
    QString diagnose();
private:
    typedef QMap<QString, QObject*> ObjectMap;
    ObjectMap objects;

    // Singleton
    ObjectPool();
    static std::unique_ptr<ObjectPool> _objectPool;
    friend ObjectPool* objectPool();
};

template <class T> T ObjectPool::find(QString id) {
    if (!objects.contains(id))
        throw Exception("Cannot find object in object pool", id);
    else {
        T object = dynamic_cast<T>(objects.value(id));
        if (!object)
            throw Exception("Object found in object pool is not of expected type", id);
        return object;
    }
}

ObjectPool* objectPool();

} //namespace

#endif
