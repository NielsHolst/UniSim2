#include "random.h"

namespace boxes {

Random *Random::theRandomGenerator = 0;

Random::Random()
{
}

QString Random::id() {
    return "Random";
}

} // namespace
