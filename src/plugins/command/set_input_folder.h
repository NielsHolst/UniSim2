#ifndef SET_INPUT_FOLDER_H
#define SET_INPUT_FOLDER_H
#include <QDir>
#include <base/command.h>

namespace command {

class set_input_folder : public base::Command
{
public:
    set_input_folder(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
