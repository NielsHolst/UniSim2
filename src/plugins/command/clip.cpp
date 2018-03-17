#include <stdio.h>
#include <QFileInfo>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "clip.h"

using namespace base;

namespace command {

PUBLISH(clip)
HELP(clip, "clip", "repeats latest output to clipboard")

clip::clip(QString name, QObject *parent)
    : Command(name, parent)
{
}


void clip::doExecute() {
    if (_args.size() > 1)
        ThrowException("'clip' takes no arguments");
    environment().recreateClipboard();
}


}
