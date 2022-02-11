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
    void reset();
    void initialize();
    void debrief();

    QString toScript();
    void addRCode(QString s);

private:
    // Input
    QString begin;
    QVector<QString> end;
    bool popUp, keepPages, keepVariables, saveAsDataFrame, plotAsList;
    double width, height;

    // Output
    base::Path ports;
    int numPages;

    // Data
    QDir _destinationDir;
    QVector<PageR*> _pages;
    QFile _file;
    QString _filePathR, _filePathTxt;
    QStringList _RCodes;

    // Methods
    QString popUpCode();
    void openFile();
    void writeScript();
    void copyToClipboard();
//    void copyFileToDestination(QString filePath);
    QString makeOutputRCode();
    QString toFigureListElement(PageR *page);
    QStringList endScripts();
};

} // boxes

#endif
