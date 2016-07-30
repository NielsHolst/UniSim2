#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "find.h"
#include "general.h"

using namespace base;

namespace command {

PUBLISH(find)
HELP(find, "find <path expression>", "finds objects matching path expression")

find::find(QString name, QObject *parent)
    : Command(name, parent)
{
}

void find::doExecute() {
    QVector<QObject*> matches;
    if (_args.size() == 2) {
        QString path = _args.at(1);
        Box *box = environment().current();
        if (box) {
            matches = box->resolveMany<QObject>(path);
            if (matches.isEmpty())
                dialog().error("No matches");
            else {
                dialog().information(info(matches));
            }
        }
        else
            dialog().error("No matches; no objects loaded");
    }
    else
        dialog().error("Write: 'find <path expression>'");
}

}
