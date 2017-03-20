#include "object_pool.h"
#include "random_generator.h"

namespace base {

RandomGenerator *RandomGenerator::theRandomGenerator = 0;

RandomGenerator::RandomGenerator()
{
}

RandomGenerator::Generator* randomGenerator() {
    if (!RandomGenerator::theRandomGenerator) {
        RandomGenerator::theRandomGenerator = new RandomGenerator;
        objectPool()->attach("RandomGenerator", RandomGenerator::theRandomGenerator);
    }
    return &(RandomGenerator::theRandomGenerator->generator);
}
void seedRandomGenerator(int seed) {
    static bool seeded = false;
    if (!seeded) {
        randomGenerator()->seed(seed);
        seeded = true;
    }
}

} // namespace
