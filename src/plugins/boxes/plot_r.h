#ifndef PLOT_R_H
#define PLOT_R_H
#include <base/box.h>
#include "output_r.h"

namespace base {
    class Port;
}

namespace boxes {

class PlotR : public base::Box
{
public:
    PlotR(QString name, QObject *parent);
    void amend();
    void reset();
    void initialize();
    QString toString();
    QString toScript();
private:
    // Inputs
    QVector<QString> ports;
    QString layout;
    int ncol, nrow;
    // Data
    QVector<base::Port*> _ports;
    // Methods
    void collectPorts();
    QString xPortLabel();
    QString dim(QString portName);
};

}

#endif
