#ifndef BASE_ENVIRONMENT_H
#define BASE_ENVIRONMENT_H

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

    QString homePath() const;

    QString openOutputFile(QFile &file, QString extension);
    QString outputFilePath(QString extension);
    QString outputFileNamePath(QString fileName);

    QString filePath(Folder folder, QString fileName) const;
    QString fileContent(Folder folder, QString fileName);
    QString folderInfo(Folder folder);

    QString latestOutputFilePath(QString fileExtension) const;
    void latestLoadArg(QString arg);
    QString latestLoadArg() const;

    QString inputFileNamePath(QString fileName) const;

    QDir dir(Folder folder) const;
    void dir(Folder folder, QString path);
    void dir(Folder folder, QDir specificDir);
    QDir resolveDir(Folder folder, Folder work = Work) const;

    void incrementFileCounter();
    void copyToClipboard(QString text);

    bool isFirstInstallation() const;
    bool isNewInstallation() const;
    void updateInstallation() const;

private:
    // Data
    Box *_root, *_current;
    QMap<Folder, QDir> _dir;
    QMap<QString,QString> _latestOutputFilePath;
    QString _latestLoadArg;
    bool _isFirstInstallation;
    // Singleton
    static Environment *_environment;
    friend Environment& environment();
    // Methods
    QString fileCounterKey();
    int fileCountervalue();
    void initDir();
    QDir findAtomDir();
    QDir findNotepadDir();
    QDir findGraphvizDir();
    void getDirSettings();
    static QDir makeDirAsNeeded(QDir dirNeeded);
};

Environment& environment();

template<class T> T convert(Environment::Folder )  { ThrowException("Cannot only convert Folder to QString"); }

template<> QString convert(Environment::Folder transform);
template<> Environment::Folder convert(QString s);

}

#endif
