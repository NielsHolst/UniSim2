#include <QMainWindow>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/publish.h>
#include "quit.h"

using namespace base;

namespace command {

PUBLISH(quit)
HELP(quit, "quit", "close console")

quit::quit(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(quit);
    helpText("quit");
}

void quit::doExecute() {
    QMainWindow *mainWindow = dynamic_cast<QMainWindow*>(dialog().parent());
    Q_ASSERT(mainWindow);
    mainWindow->close();
}

}
