#include <base/publish.h>
#include "life_fec_infected.h"

using namespace base;

namespace aphid {

PUBLISH(life_fec_infected)

life_fec_infected::life_fec_infected(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(life_fec_ala);
    Input(life_fec_apt);
    Input(reduction);
    Output(life_fec_alaInfected);
    Output(life_fec_aptInfected);
} 
void life_fec_infected::reset() {
    life_fec_alaInfected=
    life_fec_aptInfected=0;
}

void life_fec_infected::update() {
    life_fec_alaInfected=life_fec_ala*reduction;
    life_fec_aptInfected=life_fec_apt*reduction;

}


}
