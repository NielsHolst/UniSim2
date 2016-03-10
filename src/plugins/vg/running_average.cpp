/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "running_average.h"
#include <base/publish.h>

using namespace base;

namespace vg {

PUBLISH(RunningAverage)

/*! \class RunningAverage
 * \brief Computes the running average of the input
 *
 * Inputs
 * ------
 * - _initialValue_ is the value assigned to _value_ at reset [R]
 * - _value_ will be put into the running average on next call of update [R]
 * - _length_ is the number of updates the average should run over [R]
 *
 * Output
 * ------
 * - _value_ is the running average [R]
 *
 */

RunningAverage::RunningAverage(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(initialValue).equals(0.);
    Input(input).equals(0.);
    Input(length).equals(10);
    Output(value);
}

void RunningAverage::reset() {
    buffer.fill(initialValue, length);
    sum = initialValue*length;
    value = initialValue;
    ix = 0;
}

void RunningAverage::update() {
    sum += input - buffer.at(ix);
    buffer[ix] = input;
    value = sum/length;
    ix = (ix + 1) % length;
}


} //namespace

