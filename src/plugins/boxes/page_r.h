/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
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
    QVector<base::Track::Order> xAxisOrders() const;
private:
    // Inputs
    bool show, plotAsList;
    QVector<bool> test;
    QVector<QString> xAxis;
    double width, height;
    int ncol, nrow;
    QString title;
    // Data
    bool _popUp;
    QVector<PlotR*> _plots;
    static int _commonPageNumber;
    int _myPageNumber;
    QVector<base::Track::Order> _xAxisOrders;
    // Methods
    QString dim(QString portName);
};

}

#endif
