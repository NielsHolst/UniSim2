#ifndef PLOT_R_H
#define PLOT_R_H
#include <QTextStream>
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
    QString layout, guideTitle, end, endCode, ggplot, transform;
    int ncol, nrow, iteration, fontSize;
    // Methods
    QVector<base::Track*> xAxisTracks();
    QString dim(QString portName);
    void appendGgplot(QTextStream &s);
};

}

#endif
