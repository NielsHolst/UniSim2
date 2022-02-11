/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PAGE_R_H
#define PAGE_R_H
#include <QVector>
#include <base/box.h>
#include <base/path.h>

namespace boxes {

class PlotR;

class PageR : public base::Box
{
public:
    PageR(QString name, base::Box *parent);
    void initialize();
    void reset();
    QString toScript();
    QString functionName();
private:
    // Inputs
    base::Path xAxis;
    double width, height;
    int ncol, nrow, numPages;
    QString title;
    bool plotAsList, popUp, keepPages;
    // Data
    QVector<PlotR*> _plots;
    static int _commonPageNumber;
    int _myPageNumber;
    bool _doPopUp;
    // Methods
    QString dim(QString portName);
};

}

#endif
