#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include "box.h"
#include "environment.h"
#include "general.h"
#include "object_pool.h"
#include "version.h"


namespace base {

Environment *Environment::_environment = 0;

const QString PATH_NOT_SET = "/FolderNotSet";

Environment& environment() {
    if (!Environment::_environment) {
        Environment::_environment = new Environment;
        objectPool()->attach("Environment", Environment::_environment);
    }
    return *Environment::_environment;
}

Environment::Environment()
    : _root(0), _current(0)
{
    QSettings settings;
    _isFirstInstallation = !QDir(homePath()).exists();
    if (_isFirstInstallation)
        initDir();
    else
        getDirSettings();
    _latestLoadArg = settings.value("environment/latest-load-arg", QString()).toString();
    _latestOutputFilePath["txt"] = settings.value("environment/latest-output-file-path-txt", QString()).toString();
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
    settings.setValue("environment/latest-load-arg", _latestLoadArg);
    settings.setValue("environment/latest-output-file-path-txt", latestOutputFilePath("txt"));
}
Box* Environment::root() {
    return _root;
}

void Environment::root(Box *newRoot) {
    _current = _root = newRoot;
}

void Environment::deleteRoot() {
    delete _root;
    _current = _root = 0;
}

Box* Environment::current() {
    return _current;
}

void Environment::current(Box *newCurrent) {
    _current = newCurrent;
}

QString Environment::homePath() const {
    QStringList paths =
        QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    if (paths.isEmpty())
        ThrowException("Cannot find home folder");
    return paths.at(0) + "/UniversalSimulatorHome";

}

QString Environment::openOutputFile(QFile &file, QString extension) {
    QString filePath = outputFilePath(extension);
    file.setFileName(filePath);
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath).context(this);
    return filePath;
}

QString Environment::outputFilePath(QString extension) {
    QString fileName = _latestLoadArg;

    char numberFilled[16];
    sprintf(numberFilled, "%04d", fileCountervalue());

    QString ext = (extension.at(0) == '.') ? extension : ("." + extension);
    return outputFileNamePath(QFileInfo(fileName).baseName() + "_" + numberFilled + ext);
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
    QString thePath = dir.absoluteFilePath( QFileInfo(fileName).fileName() );
    _latestOutputFilePath[QFileInfo(fileName).suffix()] = thePath;
    return thePath;
}

QString Environment::filePath(Folder folder, QString fileName) const {
    if (QFileInfo(fileName).isAbsolute())
        return fileName;
    QString fileNamePath = resolveDir(folder).absoluteFilePath(fileName);
    if (!QFileInfo(fileNamePath).exists())
        ThrowException("Could not find file").value(fileNamePath);
    return fileNamePath;
}

QString Environment::fileContent(Folder folder, QString fileName) {
    QString fileNamePath = filePath(folder, fileName);
    QFile file(fileNamePath);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        ThrowException("Could not open file").value(fileNamePath);
    return QString( file.readAll() );
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
            info += "\n  '" + resolvedDir.absolutePath() +  "' will be created when needed";
        else{
            bool shouldExist = (folder == Input || folder == Script) ||
                               (folder != Input && folder != Script && folderDir.dirName() != PATH_NOT_SET);
            if (shouldExist)
                info += "\n  Warning: '" + resolvedDir.absolutePath() +  "' does not exist";
            else
                info += "\n  Warning: You have not set this folder";
        }
    }
    return info;
}

QString Environment::latestOutputFilePath(QString fileExtension) const {
    return _latestOutputFilePath.contains(fileExtension) ?
           _latestOutputFilePath.value(fileExtension) :
           QString();
}

void Environment::currentLoadArg(QString arg) {
    _currentLoadArg = arg;
}

QString Environment::currentLoadArg() const {
    return _currentLoadArg;
}

void Environment::latestLoadArg(QString arg) {
    _latestLoadArg = arg;
}

QString Environment::latestLoadArg() const {
    return _latestLoadArg;
}

QString Environment::inputFileNamePath(QString fileName) const {
    QString loadFileNamePath = filePath(Input, _currentLoadArg);
    QDir loadDir = QFileInfo(loadFileNamePath).absoluteDir();
    QString fileNamePath = loadDir.absoluteFilePath(fileName),
            cleaned = QDir::cleanPath(fileNamePath);
    return cleaned;
}

QDir Environment::dir(Folder folder) const {
    return _dir.value(folder);
}

void Environment::dir(Folder folder, QString path) {
    dir(folder, QDir(path));
}

void Environment::dir(Folder folder, QDir specificDir) {
    _dir[folder] = specificDir;
}

QDir Environment::resolveDir(Folder folder, Folder work) const {
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
    return "environment/file-counter/" + QFileInfo(_latestLoadArg).baseName();
}

void Environment::copyToClipboard(QString text) {
    QApplication::clipboard()->setText(text);
}

bool Environment::isFirstInstallation() const {
    return _isFirstInstallation;
}

bool Environment::isNewInstallation() const {
    QFile file(homePath() + "/.version");
    bool isNew(true);
    if (file.open(QIODevice::Text|QIODevice::ReadOnly)) {
        QString currentVersion = file.readAll();
        isNew = (currentVersion != versionRaw());
    }
    file.close();
    return isNew;
}

void Environment::updateInstallation() const {
    QString fileNamePath = homePath() + "/.version";
    QFile file(fileNamePath);
    if (file.open(QIODevice::Text|QIODevice::WriteOnly))
        file.write(qPrintable(versionRaw()));
    else
        ThrowException("Cannot update version file").value(fileNamePath);
}

void Environment::initDir() {
    _dir[Work] = QDir(homePath());
    _dir[Input] = "input";
    _dir[Output] = "output";
    _dir[Script] = "script";
    _dir[Atom] = findAtomDir();
    _dir[Notepad] = findNotepadDir();
    _dir[Graphviz] = findGraphvizDir();
}

QDir Environment::findAtomDir() {
    QString path = QStandardPaths::locate(QStandardPaths::RuntimeLocation,
                                          ".atom/packages/language-boxes/grammars", QStandardPaths::LocateDirectory);
    if (path.isEmpty()) {
        path = "/Applications/Atom/Contents/Resources/app/apm/templates/language/grammars";
        if (!QDir(path).exists())
            path = "";
    }

    if (path.isEmpty())
        path = PATH_NOT_SET;
    return QDir(path);
}

QDir Environment::findNotepadDir() {
    QString path = QStandardPaths::locate(QStandardPaths::RuntimeLocation,
                                          "AppData/Roaming/Notepad++", QStandardPaths::LocateDirectory);
    if (path.isEmpty())
        path = PATH_NOT_SET;
    return QDir(path);
}

QDir Environment::findGraphvizDir() {
    QString path = PATH_NOT_SET;
    return QDir(path);
}

void Environment::getDirSettings() {
    QSettings settings;
    Folder fo = Folder(0);
    while (true) {
        QString key = "environment/dir" + convert<QString>(fo);
        _dir[fo] = QDir(settings.value(key, QString(".")).toString());
        if (fo == LastFolder) break;
        fo = Folder(fo+1);
    }
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

