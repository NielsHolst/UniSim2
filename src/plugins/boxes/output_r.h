/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_R_H
#define OUTPUT_R_H
#include <QFile>
#include <QString>
#include <QVector>
#include <base/box.h>
#include <base/convert.h>

class QFile;

namespace boxes {

class PageR;

class OutputR : public base::Box
{
public:
    OutputR(QString name, QObject *parent);
    void amend();
    void initialize();
    void debrief();
    QString toString();
    QString toScript();
    void addRCode(QString s);
private:
    // Input
    QString begin, outputFileNameVariable;
    bool popUp, keepPages, keepVariables;
    double width, height;
    QVector<QString> end;
    int skipSteps;

    // Output
    int numPages;

    // Data
    QVector<PageR*> _pages;
    QFile _file;
    QString _filePathR;
    QStringList _RCodes;

    // Methods
    QString popUpCode();
    void openFile();
    void writeScript();
    void copyToClipboard();
    QString makeClipboardOutput();
    QString toFigureListElement(PageR *page);
    QStringList endScripts();
};

} // boxes

#endif
