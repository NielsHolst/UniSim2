#ifndef SAVE_GRAMMAR_NOTEPAD_H
#define SAVE_GRAMMAR_NOTEPAD_H
#include <QString>
#include "save_grammar_base.h"

namespace base {

class SaveGrammarNotepad : public SaveGrammarBase
{
public:
    SaveGrammarNotepad();
private:
    // data
    QString _output;
    // methods
    QString grammarFilePath();
    void writeBeginning();
    void writePlugins();
    void writeClasses();
    void writePorts();
    void writeAttributes();
    void writeTransforms();
    void writeConstants();
    void writeEnd();
};

}

#endif
