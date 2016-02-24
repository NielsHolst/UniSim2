#include <base/publish.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "set.h"
#include "set_font.h"
#include "set_input_folder.h"
#include "set_output_folder.h"
#include "set_script_folder.h"

using namespace base;

namespace command {

PUBLISH(set)

set::set(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(set);
}

void set::doExecute() {
    Command *command(0);
    int n = _args.size();
    if (n == 1) {
        dialog().error("Write: set <something>");
        return;
    }
    else {
        Q_ASSERT(n > 1);
        QString a1 = _args.at(1),
                a2 = (n > 2) ? _args.at(2) : "";
        if (a1 == "font")
            command = new set_font("set_font", this);
        else if (a1 == "input") {
            if (a2 == "folder")
                command = new set_input_folder("set_input_folder", this);
            else
                throw Exception("Write: 'set input folder'");
        }
        else if (a1 == "output") {
            if (a2 == "folder")
                command = new set_output_folder("set_output_folder", this);
            else
                throw Exception("Write: 'set output folder'");
        }
        else if (a1 == "script") {
            if (a2 == "folder")
                command = new set_script_folder("set_script_folder", this);
            else
                throw Exception("Write: 'set script folder'");
        }
        else
            throw Exception("Unknown command: set " + a1);
    }
    if (command) {
        command->arguments(_args);
        command->execute();
    }
}


}
