#ifndef DOC_H
#define DOC_H
#include <QFile>
#include <QTextStream>
#include <base/command.h>

namespace base {
    class Box;
}

namespace command {

class doc : public base::Command
{
public:
    doc(QString name, QObject *parent);
private:
    // data
    QFile _file;
    QTextStream _text;
    // methods
    void doExecute();
    void doDoc();
    void doLoad();
    void appendHeadings();
    void appendText(base::Box *box);
    QString qualifiedClassName(QString className);
};

}

#endif
