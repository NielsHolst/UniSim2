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

private:
    // Input
    QString begin, end, outputFileNameVariable;
    bool popUp, keepPages, keepVariables;
    double width, height;

    // Output
    int numPages;

    // Data
    QVector<PageR*> _pages;
    QFile _file;
    QString _filePathR;

    // Methods
    QString popUpCode();
    void openFile();
    void writeScript();
    void copyToClipboard();
    QString makeClipboardOutput();
};

} // boxes

#endif
