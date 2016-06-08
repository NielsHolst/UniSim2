#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include <base/save_grammar_atom.h>
#include <base/save_grammar_notepad.h>
#include "save.h"

using namespace base;

namespace command {

PUBLISH(save)
HELP(save_grammar, "save grammar", "save grammar for Atom and Notepad++ editors")

save::save(QString name, QObject *parent)
    : Command(name, parent)
{
}

void save::doExecute() {
    bool ok(true);
    if (_args.size() == 2) {
        QString arg = _args.at(1);
        if (arg == "grammar")
            saveGrammar();
        else
            ok = false;
    }
    else
        ok = false;
    if (!ok)
        dialog().error("Write: 'save grammar'");
}

void save::saveGrammar() {
    bool atomOk = environment().dir(Environment::Atom).exists(),
         notepadOk = environment().dir(Environment::Notepad).exists();
    if (atomOk)
        SaveGrammarAtom().write();
    if (notepadOk)
        SaveGrammarNotepad().write();
    if (!atomOk && !notepadOk)
        dialog().error("Nothing written.\nUse 'set folder <editor name> <path>' to locate your editor(s).\n"
                       "Atom editor:\n  " +
                       environment().folderInfo(Environment::Atom) + "\n" +
                       "Notepad++ editor:\n  " +
                       environment().folderInfo(Environment::Notepad));
}

}
