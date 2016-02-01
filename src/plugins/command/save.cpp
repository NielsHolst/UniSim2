#include <base/dialog.h>
#include <base/publish.h>
#include "save.h"
#include "save_output.h"

using namespace base;

namespace command {

PUBLISH(save)

save::save(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(save);
}

void save::execute() {
    Command *command(0);
    int n = _args.size();
    if (n == 1) {
        dialog().error("Write: save <something>");
        return;
    }
    else {
        Q_ASSERT(n > 1);
        QString a1 = _args.at(1);
        if (a1 == "output")
            command = new save_output("save_output", this);
        else
            dialog().error("Unknown command: set " + a1);
    }
    if (command) {
        command->arguments(_args);
        command->execute();
    }
}


}
