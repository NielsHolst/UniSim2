/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_R_H
#define OUTPUT_R_H
#include <QDir>
#include <QFile>
#include <QString>
#include <QVector>
#include <base/box.h>
#include <base/path.h>

class QFile;

namespace boxes {

class PageR;

class OutputR : public base::Box
{
public:
    OutputR(QString name, Box *parent);
    void amend();
    void initialize();
    void debrief();

    QString toScript();
    void addRCode(QString s);

private:
    // Input
    bool showPlots, clearPlots, clearMemory, popUp, saveDataFrame, plotAsList, skipFormats;
    int showLines;
    double width, height;
    QVector<QString> plotTypes, scripts;

    // Output
    base::Path ports;
    int numPages;

    // Data
    QDir _destinationDir;
    QVector<PageR*> _pages;
    QFile _file;
    QString _filePathR, _filePathTxt;
    QStringList _RCode;

    // Methods
    QString popUpCode();
    void openFile();
    void writeScript();
    void copyToClipboard();
    QString toFigureListElement(PageR *page);
};

} // boxes

#endif
