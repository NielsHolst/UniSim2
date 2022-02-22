/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_SELECTOR_H
#define OUTPUT_SELECTOR_H
#include <QTime>
#include <base/box.h>

namespace boxes {

class OutputSelector : public base::Box
{
public:
    OutputSelector(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
    void cleanup();
    void debrief();
private:
    // Inputs
    int skipRows, period;
    bool final, useLocalDecimalChar, skipFormats;
    // Outputs
    bool isActive, isSkipping;
    int skipSteps;
    // Data
    int _totalRowCount,
        _periodCount;
};

} // boxes

#endif
