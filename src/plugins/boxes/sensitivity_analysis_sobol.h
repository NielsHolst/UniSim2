/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SENSITIVITY_ANALYSIS_SOBOL_H
#define SENSITIVITY_ANALYSIS_SOBOL_H
#include <base/matrix.h>
#include "sensitivity_analysis_base.h"

namespace boxes {

class SensitivityAnalysisSobol : public SensitivityAnalysisBase
{
public:
    SensitivityAnalysisSobol(QString name, Box *parent);
    void initialize();
    void reset();
    void debrief();
private:
    // Input
    int iteration, bootstrapSize, &N, &k;
    // Output
    char phase;
    // Data
    base::Matrix<double> A, B, C;
    // Methods
    void reviewNumberOfSamples();
    int numberOfIterations() const;
    int numberOfSamples() const;
    void fillMatrices();
};

}

#endif
