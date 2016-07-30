#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QDir>
#include <QObject>
#include <QMap>
#include "convert.h"
#include "exception.h"

namespace base {

class Box;
class Command;

class Environment : public QObject {
public:
    enum Folder {Work, Input, Output, Script, Notepad, Atom, Graphviz};
    const Folder LastFolder = Graphviz;

    Environment();
    ~Environment();

    Box* root();
    void root(Box *newRoot);
    void deleteRoot();

    Box* current();
    void current(Box *newCurrent);

    QString openOutputFile(QFile &file, QString extension);
    QString outputFilePath(QString extension);
    QString outputFileNamePath(QString fileName);

    QString filePath(Folder folder, QString fileName);
    QString fileContent(Folder folder, QString fileName);
    QString folderInfo(Folder folder);

    QString latestOutputFilePath(QString fileExtension);

    QDir dir(Folder folder);
    void dir(Folder folder, QString path);
    void dir(Folder folder, QDir specificDir);
    QDir resolveDir(Folder folder, Folder work = Work);

    void incrementFileCounter();
    void copyToClipboard(QString text);

    struct {
        bool autosave;
        QString latestLoadArg;
        Command *command;
    } state;
private:
    // Data
    Box *_root, *_current;
    QMap<Folder, QDir> _dir;
    QMap<QString,QString> _latestOutputFilePath;
    // Singleton
    static Environment *_environment;
    friend Environment& environment();
    // Methods
    QString fileCounterKey();
    int fileCountervalue();
    static QDir makeDirAsNeeded(QDir dirNeeded);
};

Environment& environment();

template<class T> T convert(Environment::Folder )  { ThrowException("Cannot only convert Folder to QString"); }

template<> QString convert(Environment::Folder transform);
template<> Environment::Folder convert(QString s);

}

#endif
