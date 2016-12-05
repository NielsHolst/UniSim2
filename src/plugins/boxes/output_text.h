#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H
#include <QFile>
#include <QString>
#include <QVector>
#include <base/box.h>
#include <base/convert.h>

class QFile;

namespace boxes {

class PageR;

class OutputText : public base::Box
{
public:
    OutputText(QString name, QObject *parent);

private:
    // Input
    QVector<QString> ports;
};

} // boxes

#endif
