#ifndef PAGE_R_H
#define PAGE_R_H
#include <QVector>
#include <base/box.h>
#include <base/track.h>

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
    base::Track::Order xAxisOrder() const;
private:
    // Inputs
    QVector<QString> xAxis;
    double width, height;
    int ncol, nrow;
    // Data
    bool _popUp;
    QVector<PlotR*> _plots;
    static int _commonPageNumber;
    int _myPageNumber;
    base::Track::Order _xAxisOrder;
    // Methods
    QString dim(QString portName);
};

}

#endif
