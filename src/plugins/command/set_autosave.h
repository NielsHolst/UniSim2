#ifndef SET_AUTOSAVE_H
#define SET_AUTOSAVE_H
#include <base/command.h>

namespace command {

class set_autosave : public base::Command
{
public:
    set_autosave(QString name, QObject *parent);
private:
    void doExecute();
    bool value(QString s);
};

}

#endif
