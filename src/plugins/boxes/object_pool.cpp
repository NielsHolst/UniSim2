#include "mega_factory.h"
#include "object_pool.h"

namespace boxes {

std::unique_ptr<ObjectPool> ObjectPool::_objectPool(new ObjectPool);

ObjectPool* objectPool() {
    return ObjectPool::_objectPool.get();
}

ObjectPool::ObjectPool()
{
    attach(MegaFactory::id(), new MegaFactory);
}

void ObjectPool::attach(QString id, QObject *object) {
    objects[id] = object;
    object->setParent(this);
}

bool ObjectPool::contains(QString id) {
    return objects.contains(id);
}


} //namespace

