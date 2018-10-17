#include <base/box.h>
#include <base/box_output.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "list.h"
#include "list_output.h"

using namespace base;

namespace command {

PUBLISH(list)
HELP(list, "list", "lists box structure starting from root")

list::list(QString name, QObject *parent)
    : Command(name, parent)
{
}

void list::doExecute() {
    // Drop "list"
    _args.removeAt(0);

    // Find boxes
    QString path = _args.isEmpty() ?  QString(".") : _args.first();

    if (path == "?")
        showHelp();
    else {
        Box *current = environment().current();
        if (!current) {
            dialog().error("No script loaded");
            return;
        }
        QVector<Box*> boxes = current->findMany<Box>(path);
        if (boxes.isEmpty()) {
            dialog().error("Nothing matches: '" + path +"'");
            return;
        }
        // Set option set
        if (!_args.isEmpty())
            _args.removeAt(0);
        QString ops = _args.join("");
        _options = convert<ListOptionSet>(ops);

        // Show output
        ListOutput output(boxes, _options);
        dialog().information(output.toString());
    }
}

void list::showHelp() {
    QString s = "Use one of three formats:\n"
                "list (which defaults to list . br) -or-\n"
                "list <path> (which defaults to list <path> br) -or-\n"
                "list <path> <options>"
            "\n\n<options> (which can be combined) are\n"
            "p (to show Ports)\n"
            "i (to show Input ports)\n"
            "o (to show Output ports)\n"
            "m (to show iMported ports)\n"
            "x (to show eXported ports)\n"
            "b (to show Boxes)\n"
            "r (to show Recursively)";
     dialog().information(s);
}

}
