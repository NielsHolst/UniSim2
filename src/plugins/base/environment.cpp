#include <stdio.h>
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include "environment.h"
#include "general.h"
#include "object_pool.h"


namespace base {

Environment *Environment::_environment = 0;

Environment& environment() {
    if (!Environment::_environment) {
        Environment::_environment = new Environment;
        objectPool()->attach("Environment", Environment::_environment);
    }
    return *Environment::_environment;
}

Environment::Environment() {
    QSettings settings;
    Folder fo = Folder(0);
    while (true) {
        QString key = "environment/dir" + convert<QString>(fo);
        _dir[fo] = QDir(settings.value(key, QString(".")).toString());
        if (fo == LastFolder) break;
        fo = Folder(fo+1);
    }
    state.autosave = settings.value("environment/autosave", true).toBool();
    state.latestLoadArg = settings.value("environment/latest-load-arg", QString()).toString();
    state.latestOutputFilePath = settings.value("environment/latest-output-file-path", QString()).toString();
    state.root = 0;
    state.command = 0;
}

Environment::~Environment() {
    QSettings settings;
    Folder fo = Folder(0);
    while (true) {
        QString key = "environment/dir" + convert<QString>(fo);
        settings.setValue(key, _dir.value(fo).path());
        if (fo == LastFolder) break;
        fo = Folder(fo+1);
    }
    settings.setValue("environment/autosave", state.autosave);
    settings.setValue("environment/latest-load-arg", state.latestLoadArg);
    settings.setValue("environment/latest-output-file-path", state.latestOutputFilePath);
}

void Environment::openOutputFile(QFile &file, QString extension) {
    QString filePath = outputFilePath(extension);
    file.setFileName(filePath);
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath).context(this);
}

QString Environment::outputFilePath(QString extension) {
    QString fileName = state.latestLoadArg;
    int n = fileName.lastIndexOf(".");

    char numberFilled[16];
    sprintf(numberFilled, "%04d", fileCountervalue());

    QString ext = (extension.at(0) == '.') ? extension : ("." + extension);
    return outputFileNamePath(fileName.left(n) + "_" + numberFilled + ext);
}

QString Environment::outputFileNamePath(QString fileName) {
    // Resolve dir paths in output and in fileName
    QString fileNameDirPath = QFileInfo(fileName).path();
    bool fileNameHasDir = !fileNameDirPath.isEmpty();
    QDir outputDir = resolveDir(Output),
         fileNameDir = QDir(fileNameDirPath);
    bool fileNameHasAbsoluteDir = fileNameHasDir && fileNameDir.isAbsolute();

    // Construct path to output dir
    QString outputDirPath;
    if (fileNameHasDir)
        outputDirPath = fileNameHasAbsoluteDir ?
                        fileNameDir.absolutePath() :
                        outputDir.absolutePath() + "/" + fileNameDirPath;
    else
        outputDirPath = outputDir.absolutePath();
    outputDirPath.replace("\\", "/");

    // Create dir as needed and return complete absolute file name path
    QDir dir = makeDirAsNeeded(outputDirPath);
    return state.latestOutputFilePath = dir.absoluteFilePath( QFileInfo(fileName).fileName() );
}

QString Environment::filePath(Folder folder, QString fileName) {
    QString fileNamePath = resolveDir(folder).absoluteFilePath(fileName);
    if (!QFileInfo(fileNamePath).exists())
        ThrowException("Could not find file").value(fileNamePath);
    return fileNamePath;
}

QString Environment::folderInfo(Folder folder) {
    QString info;
    QDir folderDir = dir(folder),
         resolvedDir = resolveDir(folder);

    if (folderDir.isRelative()) {
        info = "relative path '%1' resolves to '%2'";
        info = info.arg(folderDir.path()).arg(resolvedDir.absolutePath());
    }
    else {
        info = "absolute path '%1'";
        info = info.arg(folderDir.absolutePath());
    }
    if (!resolvedDir.exists()) {
        if (folder == Output)
            info += "\n'" + resolvedDir.absolutePath() +  "' will be created when needed";
        else
            info += "\nWarning: '" + resolvedDir.absolutePath() +  "' does not exist";
    }
    return info;
}

QDir Environment::dir(Folder folder) {
    return _dir.value(folder);
}

void Environment::dir(Folder folder, QString path) {
    dir(folder, QDir(path));
}

void Environment::dir(Folder folder, QDir specificDir) {
    _dir[folder] = specificDir;
}

QDir Environment::resolveDir(Folder folder, Folder work) {
    if (folder == Work)
        return dir(work);

    QDir workDir = dir(work);
//    if (!work.isAbsolute())
//        ThrowException("Work folder must be an absolute path").value(work.path());

    QDir specificDir = dir(folder);
    QString path = (specificDir.isAbsolute()) ?
                specificDir.absolutePath() :
                (workDir.absolutePath() + "/" + specificDir.path());
    return QDir(path);
}

QDir Environment::makeDirAsNeeded(QDir dirNeeded) {
    if (!dirNeeded.mkpath("."))
        ThrowException("Could not create folder").value(dirNeeded.path());
    return dirNeeded;
}

void Environment::incrementFileCounter() {
    QSettings settings;
    int number = fileCountervalue();
    settings.setValue(fileCounterKey(), ++number);
}

int Environment::fileCountervalue() {
    QSettings settings;
    bool ok;
    int number = settings.value(fileCounterKey(), 0).toInt(&ok);
    return ok ? number : 0;
}

QString Environment::fileCounterKey() {
    return "environment/file-counter/" + QFileInfo(state.latestLoadArg).baseName();
}

void Environment::copyToClipboard(QString text) {
    QApplication::clipboard()->setText(text);
}


#define FOLDER_CASE(X) \
    case Environment::X: \
        s = #X; \
        break

#define FOLDER_ENTRY(X) \
    _map[#X] = Environment::X

namespace {
    const QMap<QString, Environment::Folder>& folderMap() {
        static QMap<QString, Environment::Folder> _map;
        if (_map.isEmpty()) {
            FOLDER_ENTRY(Work);
            FOLDER_ENTRY(Input);
            FOLDER_ENTRY(Output);
            FOLDER_ENTRY(Script);
            FOLDER_ENTRY(Notepad);
            FOLDER_ENTRY(Atom);
            FOLDER_ENTRY(Graphviz);
        }
        return _map;
    }
}


template<> QString convert(Environment::Folder folder) {
    QString s;
    switch (folder) {
        FOLDER_CASE(Work);
        FOLDER_CASE(Input);
        FOLDER_CASE(Output);
        FOLDER_CASE(Script);
        FOLDER_CASE(Notepad);
        FOLDER_CASE(Atom);
        FOLDER_CASE(Graphviz);
    }
    return s;
}

template<> Environment::Folder convert(QString s) {
    s = s.toLower();
    s[0] = s.at(0).toUpper();
    if (folderMap().contains(s))
        return folderMap().value(s);
    ThrowException("Unknown folder").value(s);
}

}

