#include "exception.h"
#include "object_pool.h"

namespace base {

ObjectPool *ObjectPool::_objectPool = 0;

ObjectPool::ObjectPool(QObject *parent)
    : QObject(parent)
{
    // Allow only one instance of object pool
    Q_ASSERT(!_objectPool);
    // Must have parent (to take care of destruction)
    Q_ASSERT(parent);
    _objectPool = this;
}

void ObjectPool::attach(QString id, QObject *object) {
    objects[id] = object;
    // Attached object will be deleted when its parent is deleted
    object->setParent(this);
}

bool ObjectPool::contains(QString id) {
    return objects.contains(id);
}

ObjectPool* objectPool() {
    Q_ASSERT(ObjectPool::_objectPool);
    return ObjectPool::_objectPool;
}


} //namespace

