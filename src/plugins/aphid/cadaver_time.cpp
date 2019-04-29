#include <base/publish.h>
#include "cadaver_time.h"
#include <base/vector_op.h>

using vector_op::sum;
using namespace base;

namespace aphid {

PUBLISH(CadaverTime)

CadaverTime::CadaverTime(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes day-degree time step accelerated by high RH");
    Input(isSporulating).help("Are cadavers sporulating?").unit("boolean");
    Input(timeStep).help("Time step").unit("DD");
    Input(rhAccelaration).help("Acceleration factor above RH threshold").unit("-");
    Output(step).help("RH-corrected time step").unit("DD");
    Output(total).help("Accumulated RH-corrected time steps since reset").unit("DD");
}

void CadaverTime::update() {
    step = isSporulating ? rhAccelaration*timeStep : timeStep;
    total += step;
}

}


