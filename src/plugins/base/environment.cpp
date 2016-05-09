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
    state.dir.work = QDir(settings.value("environment/dir-work", QString(".")).toString());
    state.dir.input = QDir(settings.value("environment/dir-input", QString(".")).toString());
    state.dir.output = QDir(settings.value("environment/dir-output", QString(".")).toString());
    state.dir.script = QDir(settings.value("environment/dir-script", QString(".")).toString());
    state.autosave = settings.value("environment/autosave", true).toBool();
    state.latestLoadArg = settings.value("environment/latest-load-arg", QString()).toString();
    state.latestOutputFilePath = settings.value("environment/latest-output-file-path", QString()).toString();
    state.root = 0;
    state.command = 0;
}

Environment::~Environment() {
    QSettings settings;
    settings.setValue("environment/dir-work", state.dir.work.path());
    settings.setValue("environment/dir-input", state.dir.input.path());
    settings.setValue("environment/dir-output", state.dir.output.path());
    settings.setValue("environment/dir-script", state.dir.script.path());
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
    QDir dir = makeDir(state.dir.work, state.dir.output);
    QString dirInFileName = QFileInfo(fileName).path();
    if (!dirInFileName.isEmpty())
        dir = makeDir(dir, dirInFileName);
    QString folderPath = dir.absolutePath().replace("\\", "/");
    return state.latestOutputFilePath = folderPath + "/" + QFileInfo(fileName).fileName();
}

QString Environment::scriptFilePath(QString fileName) {
    QDir dir = locateDir(state.dir.work, state.dir.script);
    return dir.absoluteFilePath(fileName);
}

QDir Environment::makeDir(QDir baseDir, QDir specificDir) {
    QDir dir = locateDir(baseDir, specificDir);
    if (!dir.mkpath("."))
        ThrowException("Could not create folder").value(dir.path());
    return dir;
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

}

