#include "mega_factory.h"
#include "object_pool.h"

namespace base {

std::unique_ptr<ObjectPool> ObjectPool::_objectPool(new ObjectPool);

ObjectPool* objectPool() {
    return ObjectPool::_objectPool.get();
}

ObjectPool::ObjectPool()
{
}

void ObjectPool::attach(QString id, QObject *object) {
    objects[id] = object;
    // Attached object will be deleted when its parent is deleted
    object->setParent(this);
}

bool ObjectPool::contains(QString id) {
    return objects.contains(id);
}


} //namespace

