#include <QMainWindow>
#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "quit.h"

using namespace base;

namespace command {

PUBLISH(quit)

quit::quit(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(quit);
}

void quit::execute(DialogBase *dialog, Environment *) {
    QMainWindow *mainWindow = dynamic_cast<QMainWindow*>(dialog->parent());
    if (mainWindow)
        mainWindow->close();
    else
        dialog->error("Cannot close main window");
}

}
