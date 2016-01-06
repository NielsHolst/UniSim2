#include "port.h"
#include "port_cache.h"

namespace boxes {

// Singleton

std::unique_ptr<PortCache> PortCache::_portCache(new PortCache);

PortCache* portCache() {
    return PortCache::_portCache.get();
}

// Class definition

#define CACHE_CREATE(Y) \
    cache[Y] = new Port("", this); \
    cache.value(Y)->data(&v##Y); \
    cache[Y##Vector] = new Port("", this); \
    cache.value(Y##Vector)->data(&vv##Y);

PortCache::PortCache()
{
    CACHE_CREATE(Bool)
    CACHE_CREATE(Char)
    CACHE_CREATE(Int)
    CACHE_CREATE(LongInt)
    CACHE_CREATE(LongLongInt)
    CACHE_CREATE(Float)
    CACHE_CREATE(Double)
    CACHE_CREATE(LongDouble)
    CACHE_CREATE(String)
    CACHE_CREATE(Date)
    CACHE_CREATE(Time)
    CACHE_CREATE(DateTime)
}

Port* PortCache::port(PortType type) {
    Q_ASSERT(cache.contains(type));
    return cache.value(type);
}


}
