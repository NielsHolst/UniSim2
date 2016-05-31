#ifndef SAVE_GRAMMAR_BASE_H
#define SAVE_GRAMMAR_BASE_H
#include <QFile>
#include <QObject>
#include <QSet>
#include <QTextStream>

namespace base {

class Box;

class SaveGrammarBase : public QObject
{
public:
    SaveGrammarBase();
    void write();
protected:
    // data
    QFile _file;
    QTextStream _stream;
    // methods
    QStringList plugInNames();
    QStringList classNames();
    QStringList portNames();
    QStringList attributeNames();
    QStringList transformNames();
    QStringList constantNames();
private:
    // methods
    void openFileStream();
    void closeFileStream();
    QSet<QString> collectPortNames(const Box *root);
    virtual QString grammarFilePath() = 0;
    virtual void writeBeginning() = 0;
    virtual void writePlugins() = 0;
    virtual void writeClasses() = 0;
    virtual void writePorts() = 0;
    virtual void writeAttributes() = 0;
    virtual void writeTransforms() = 0;
    virtual void writeConstants() = 0;
    virtual void writeEnd() = 0;
};

}

#endif
