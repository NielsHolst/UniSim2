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
    double width, height;
    int ncol, nrow;
    bool popUp;
    // Data
    QVector<PlotR*> _plots;
    static int _commonPageNumber;
    int _myPageNumber;
    // Methods
    QString dim(QString portName);
};

}

#endif
