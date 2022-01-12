/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SENSITIVITY_ANALYSIS_SIMPLE_H
#define SENSITIVITY_ANALYSIS_SIMPLE_H
#include "sensitivity_analysis_base.h"


namespace boxes {

class SensitivityAnalysisSimple : public SensitivityAnalysisBase
{
public:
    SensitivityAnalysisSimple(QString name, Box *parent);
    void reset();
private:
    // Inputs
    QString method;
    int simIterations;
    // Methods
    int numberOfIterations() const;
    int numberOfSamples() const;
};

}

#endif
