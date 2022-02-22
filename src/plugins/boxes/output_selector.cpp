/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include <base/exception.h>
#include <base/publish.h>
#include "output_selector.h"

using namespace base;

namespace boxes {

PUBLISH(OutputSelector)

OutputSelector::OutputSelector(QString name, Box *parent)
    : Box(name, parent)
{
    help("selects what to output");
    Input(skipRows).equals(0).help("The initial `skipRows` steps in every iteration will be ignored");
    Input(period).equals(1).help("If >1: A row of summary output will be produced with this period");
    Input(final).equals(false).help("Overrules 'period'");
    Input(useLocalDecimalChar).equals(false).help("Use local decimal character in output?");
    Input(skipFormats).equals(false).help("Skip line with column formats?");
    Output(isActive).help("Should output be written?");
    Output(isSkipping).help("Are lines being skipped?");
    Output(skipSteps).help("Number of simulation steps skipped");
}

void OutputSelector::initialize() {
    isSkipping = (skipRows > 0);
    isActive = false;
    skipSteps = skipRows*period;
}

void OutputSelector::reset() {
    _totalRowCount =
    _periodCount   = 0;
    update();
}

void OutputSelector::update() {
    isSkipping = (++_totalRowCount < skipRows);
    isActive = !(isSkipping || final) ? (++_periodCount % period == 0) : false;
}

void OutputSelector::cleanup() {
    isSkipping = false;
    isActive = final;
}

void OutputSelector::debrief() {
    isSkipping = false;
    isActive = false;
}

}
