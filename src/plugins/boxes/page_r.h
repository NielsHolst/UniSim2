#ifndef PAGE_R_H
#define PAGE_R_H
#include <base/box.h>

namespace boxes {

class PlotR;

class PageR : public base::Box
{
public:
    PageR(QString name, QObject *parent);
    void amend();
    void reset();
    void initialize();
    QString toString();
    QString toScript();
    QString functionName();
private:
    // Inputs
    QString xAxis;
    int width, height, ncol, nrow;
    bool useRStudio;
    // Data
    QVector<PlotR*> _plots;
    static int _commonPageNumber;
    int _myPageNumber;
    // Methods
    QString dim(QString portName);
};

}

#endif
