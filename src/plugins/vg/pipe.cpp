/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <QStringList>
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
    Input(material).equals("carbon steel").help("Material type").unit("text id");
    Input(density).equals(1.).help("Pipe length per greenhouse area").unit("m/m2");
    Input(diameter).equals(51.).help("Pipe inner diameter").unit("mm");
    Input(flowRate).equals(5.).help("Water flow rate").unit("L/s");
    Input(minTemperature).equals(20.).help("Minimum inflow temperature").unit("oC");
    Input(maxTemperature).equals(80.).help("Maximum inflow temperature").unit("oC");
    Output(a).help("Heat pipe effect coefficient").unit("W/m/mm");
    Output(b).help("Heat pipe effect coefficient").unit("!=1");
    Output(emissivity).help("Emissivity of pipe").unit("[0;1]");
    materialInputs["carbon steel"] = MaterialInputs{0.0154, 1.253, 0.8};
    materialInputs["polyethylene"] = MaterialInputs{0.0123, 1.281, 0.1};
    materialInputs["copper"]       = MaterialInputs{0.0154, 1.253, 0.2};
    materialInputs["aluminium"]    = MaterialInputs{0.0154, 1.253, 0.2};
    parseMaterial();
}

void Pipe::reset() {
    parseMaterial();
    update();
}

void Pipe::update() {
    if (TestNum::eq(flowRate, -999)) flowRate = 5.;
}

void Pipe::parseMaterial() {
    QString s = material.toLower();
    if (!materialInputs.contains(s))
        ThrowException("Unknown pipe material").value(material).
                hint(QStringList(materialInputs.keys()).join(",")).context(this);
    a = materialInputs.value(s).a;
    b = materialInputs.value(s).b;
    emissivity = materialInputs.value(s).emissivity;
}

}

