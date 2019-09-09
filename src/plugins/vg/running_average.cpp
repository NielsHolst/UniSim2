/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "running_average.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(RunningAverage)

RunningAverage::RunningAverage(QString name, QObject *parent)
	: Box(name, parent)
{
    help("computes a running average");
    Input(resetValue).help("Value of running average at reset");
    Input(input).help("Input to add to running average");
    Input(length).equals(10).help("Number of inputs to average").unit("-");
    Output(value).help("Running average");
}

void RunningAverage::reset() {
    buffer.fill(resetValue, length);
    sum = resetValue*length;
    value = resetValue;
    ix = 0;
}

void RunningAverage::update() {
    sum += input - buffer.at(ix);
    buffer[ix] = input;
    value = sum/length;
    ix = (ix + 1) % length;
}


} //namespace

