#include <base/matrix.h>
#include <base/mega_factory.h>
#include <base/publish.h>
#include "big_bang.h"

using namespace base;

namespace student {

PUBLISH(BigBang)

BigBang::BigBang(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(connectivityFile).help("Name of file with connectivity matrix");
}

void BigBang::amend() {
    // Read connectivity matrix
    Matrix<double> C;
    C.read(connectivityFile, Table::NoLabelling);
    // Create plant objects
    for (int i=0; i<C.numRow(); ++i) {
        QString name = "Plant" + QString::number(i);
        plants << MegaFactory::create<Box>("student::Plant", name, this);
    }
    // Create pollinator objects
    for (int i=0; i<C.numCol(); ++i) {
        QString name = "Pollinator" + QString::number(i);
        pollinators << MegaFactory::create<Box>("student::Pollinator", name, this);
    }
}

void BigBang::reset() {
    // Assign random numbers to plants and pollinators
    for (Box *plant : plants)
        setPlantParameters(plant);
    for (Box *pollinator : pollinators)
        setPollinatorParameters(pollinator);
}

void BigBang::setPlantParameters(Box *plant) {
    // Draw random numbers from boxes which must be declared in the box script
    plant->port("initial_R")->equals( draw("random/plantInitialMass") );
    // and maybe more...
}

void BigBang::setPollinatorParameters(Box *pollinator) {
    // Draw random numbers from boxes which must be declared in the box script
    pollinator->port("initial_B")->equals( draw("random/pollinatorInitialMass") );
    pollinator->port("m")->equals( draw("random/pollinatorBodyMass") );
    pollinator->port("is_vertebrate")->equals( draw("random/isVertebrate")<0 );
    // and maybe more...
}

double BigBang::draw(QString boxPath) {
    // Find the box with the random generator
    Box *random = findOne<Box>(boxPath);
    // Update the box, so that it has a new random value ready
    random->update();
    // Return the random number from its value port
    return random->port("value")->value<double>();
}

}
