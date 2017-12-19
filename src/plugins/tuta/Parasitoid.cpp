#include <base/publish.h>
#include "Parasitoid.h"

using namespace base;

namespace tuta {

PUBLISH(Parasitoid)

Parasitoid::Parasitoid(QString name, QObject *parent)
    : Box(name, parent)
{
    help("Releasing parasitoid every week");
    Input(date).help("initial date");
    Input(firstDate).help("releasing date");
    Input(number).equals(0).help("number of parasitoid");
    Output(value).help("Value of number of parasitoid");
}

void Parasitoid::update() {

    int days = firstDate.daysTo(date);

    if (days % 7 == 0)
        value = number;
    else
        value = 0;
}


}
