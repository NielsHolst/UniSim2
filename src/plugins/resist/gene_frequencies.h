#ifndef GENE_FREQUENCIES_H
#define GENE_FREQUENCIES_H

#include <base/box.h>

namespace resist {

class GeneFrequencies : public base::Box
{
public:
    GeneFrequencies(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double> N;
    double rThreshold;
    int step, maxStep;
    // Outputs
    double ss, sr, rr, s, r;
    int thresholdGen;
    bool thresholdPassed;
};

}

#endif
