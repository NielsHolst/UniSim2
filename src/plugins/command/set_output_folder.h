#ifndef SET_OUTPUT_FOLDER_H
#define SET_OUTPUT_FOLDER_H
#include <QDir>
#include <base/command.h>

namespace command {

class set_output_folder : public base::Command
{
public:
    set_output_folder(QString name, QObject *parent);
    void execute();
};

}

#endif
