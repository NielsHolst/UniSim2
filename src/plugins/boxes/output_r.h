#ifndef OUTPUT_R_H
#define OUTPUT_R_H
#include <QFile>
#include <QString>
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
    QString xAxis, layout;
    int width, height;
    bool clear;

    // Data
    QVector<PageR*> _pages;
    QFile _file;

    // Methods
    void collectInfo();
    void setTrackX();
    void writeScript();
    void openFile();
};

} // boxes

#endif
