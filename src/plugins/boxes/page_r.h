#ifndef PAGE_R_H
#define PAGE_R_H
#include <base/box.h>

namespace boxes {

class PlotR;

class PageR : public base::Box
{
public:
    PageR(QString name, QObject *parent);
    void initialize();
    QString toString();
    QString toScript();
    QString functionName() const;
private:
    // Inputs
    QString xAxis, layout;
    int width, height, ncol, nrow;
    // Data
    QVector<PlotR*> _plots;
    // Methods
    QString dim(QString portName);
};

}

#endif
