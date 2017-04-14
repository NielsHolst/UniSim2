#ifndef PLOT_R_H
#define PLOT_R_H
#include <base/track.h>
#include "output_ports.h"
#include "output_r.h"

namespace base {
    class Port;
}

namespace boxes {

class PlotR : public OutputPorts
{
public:
    PlotR(QString name, QObject *parent);
    void reset();
    void initialize();
    QString toString();
    QString toScript();
private:
    // Inputs
    bool hide;
    QVector<QString> ports;
    QString layout, end, endCode, iterationId;
    int ncol, nrow, iteration;
    // Methods
    base::Track *xAxisTrack();
    QString dim(QString portName);
};

}

#endif
