#include <QStringListIterator>
#include <QTextStream>
#include "environment.h"
#include "exception.h"
#include "save_grammar_notepad.h"

namespace base {

namespace {
    const QString sep = "&#x000D;&#x000A;";
}

SaveGrammarNotepad::SaveGrammarNotepad()
    : SaveGrammarBase()
{
}

QString SaveGrammarNotepad::grammarFilePath() {
    QDir dir = environment().dir(Environment::Notepad);
    return dir.absoluteFilePath("userDefineLang.xml");
}

void SaveGrammarNotepad::writeBeginning() {
    QFile notepad_template(":notepad_grammar_template.xml");
    if ( !notepad_template.open(QIODevice::ReadOnly | QIODevice::Text) )
        ThrowException("Cannot open file").context(this);

    QTextStream stream(&_output);
    stream
        << notepad_template.readAll();
}

void SaveGrammarNotepad::writePlugins() {
    _output = _output.replace("#Keywords1", plugInNames().join(sep));
}

void SaveGrammarNotepad::writeClasses() {
    _output = _output.replace("#Keywords2", classNames().join(sep));
}

void SaveGrammarNotepad::writePorts() {
    QStringList decoratedPortNames;
    QStringListIterator it(portNames());
    while (it.hasNext()) {
        QString s = it.next();
        decoratedPortNames << "." + s; // << "["  + s + "]"; does not highlight port
    }
    _output = _output.replace("#Keywords3", decoratedPortNames.join(sep));
}

void SaveGrammarNotepad::writeAttributes() {
    _output = _output.replace("#Keywords4", attributeNames().join(sep));
}

void SaveGrammarNotepad::writeTransforms() {
    _output = _output.replace("#Keywords5", transformNames().join(sep));
}

void SaveGrammarNotepad::writeConstants() {
    _output = _output.replace("#Keywords6", constantNames().join(sep));
}

void SaveGrammarNotepad::writeEnd() {
    _stream <<_output;
}

}
