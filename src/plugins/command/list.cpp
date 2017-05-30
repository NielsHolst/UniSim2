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
    _args.removeFirst();

    // Find boxes
    QString path = _args.isEmpty() ?  QString(".") : _args.first();
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
        _args.removeFirst();
    QString ops = _args.join("");
    _options = convert<ListOptionSet>(ops);
    // Show output
    ListOutput output(boxes, _options);
    dialog().information(output.toString());
}

}
