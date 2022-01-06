/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "screen_combination.h"

using namespace base;

namespace vg {

PUBLISH(ScreenCombination)

ScreenCombination::ScreenCombination(QString name, QObject *parent)
    : Box(name, parent)
{
    help("combines states of screens");
    Input(formula).help("Formula of the form, e.g., 'shade1+energy2'");
    Input(screenControllersPath).equals("controllers/screens").help("Path to controllers");
    Output(value).help("Combined setting").unit("[0;1]");
}

void ScreenCombination::reset() {
    update();
}

void ScreenCombination::update() {
    // Find screen states
    if (formula != _oldFormula) {
        _states.clear();
        QStringList names = formula.split("+");
        for (QString name : names) {
            Box *controller = findOne<Box*>(screenControllersPath+ "/" + name);
            _states << controller->port("value")->valuePtr<double>();
        }
        _oldFormula = formula;
    }
    // Find max state
    value = 0.;
    for (const double *state : _states) {
        if (*state > value)
            value = *state;
    }
}

} //namespace

