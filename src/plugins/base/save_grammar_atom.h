#ifndef SAVE_GRAMMAR_ATOM_H
#define SAVE_GRAMMAR_ATOM_H
#include <QString>
#include "save_grammar_base.h"

namespace base {

class SaveGrammarAtom : public SaveGrammarBase
{
public:
    SaveGrammarAtom();
private:
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
    void writePattern(QStringList match, QString name);
};

}

#endif
