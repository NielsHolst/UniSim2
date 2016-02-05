#include <base/dialog.h>
#include <base/exception.h>
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

void save::doExecute() {
    Command *command(0);
    int n = _args.size();
    if (n == 1) {
        throw Exception("Write: save <something>");
    }
    else {
        Q_ASSERT(n > 1);
        QString a1 = _args.at(1);
        if (a1 == "output")
            command = new save_output("save_output", this);
        else
            throw Exception("Unknown command: set " + a1);
    }
    if (command) {
        command->arguments(_args);
        command->execute();
    }
}


}
