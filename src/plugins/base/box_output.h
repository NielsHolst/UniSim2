#ifndef BOX_OUTPUT_H
#define BOX_OUTPUT_H

#include <QObject>
#include <QStack>
#include <QString>

namespace base {

class Box;

class BoxOutput
{
public:
    enum Format{Indented, List};
    BoxOutput(const Box *box, Format format);
    QString asText();
private:
    const Box *_box;
    Format _format;
    int _level;
    QString _output;

    void addOutput(const QObject *box);
    QString entry(const QObject *box);
    QString beforeEntry();
    QString afterEntry();
    QString indent();

    QString beginLevel();
    QString endLevel();
};

}
#endif
