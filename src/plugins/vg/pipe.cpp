/* Copyright 2005-2018 by
** Niels Holst, Aarhus University [niels.holst@agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner@igzev.de].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/path.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "general.h"
#include "pipe.h"

using std::min;
using std::max;
using base::bounded;

using namespace base;

namespace vg {
	
PUBLISH(Pipe)

Pipe::Pipe(QString name, QObject *parent)
	: Box(name, parent)
{
    help("holds heat pipe characteristics");
    Input(density).equals(1.).help("Pipe length per greenhouse area [m/m2]");
    Input(diameter).equals(51.).help("Pipe inner diameter [mm]");
    Input(flowRate).equals(5.).help("Water flow rate [L/s]");
    Input(minTemperature).equals(20.).help("Minimum inflow temperature [oC]");
    Input(maxTemperature).equals(80.).help("Maximum inflow temperature [oC]");
    Input(a).equals(0.0154).help("Heat pipe effect coefficient [W/m/mm]");
    Input(b).equals(1.253).help("Heat pipe effect coefficient (!=1)");
    Input(emissivity).equals(0.9).help("Pipe emissivity [0;1] (not used?)");
}

}


