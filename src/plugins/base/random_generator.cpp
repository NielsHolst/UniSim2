#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/random.hpp>
#pragma GCC diagnostic pop

#include "object_pool.h"
#include "random_generator.h"

namespace base {

RandomGenerator *RandomGenerator::theRandomGenerator = 0;

RandomGenerator::RandomGenerator()
{
}

unsigned RandomGenerator::Shuffler::operator()(unsigned i) {
    boost::uniform_int<> rng(0, i - 1);
    return rng(*randomGenerator());
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
