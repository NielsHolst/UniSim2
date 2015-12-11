#ifndef BOX_OUTPUT_H
#define BOX_OUTPUT_H

#include <QStack>
#include <QString>

namespace boxes {

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
    QString name(const QObject *box);
    QString beforeName();
    QString afterName();
    QString indent();

    QString beginLevel();
    QString endLevel();
};

}
#endif
