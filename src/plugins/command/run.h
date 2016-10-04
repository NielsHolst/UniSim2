#ifndef RUN_H
#define RUN_H
#include <base/command.h>

namespace base {
    class Box;
    class Port;
}

namespace command {

class run : public base::Command
{
public:
    run(QString name, QObject *parent);
private:
    // Data
    base::Box *_root;
    // Methods
    void doExecute();
    void doLoad();
    void doRun();
    void doSave();
    QString messageTime() const;
    QString message(QString counter, QString total, QString in) const;
    QString errorMessage() const;
};

}

#endif
