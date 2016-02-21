#include <stdio.h>
#include <QApplication>
#include <QClipboard>
#include <QDir>
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

QString Environment::outputFilePath(QString extension) {
    QDir dir = makeDir(environment().state.dir.work, environment().state.dir.output);
    QString folderPath = dir.absolutePath();
    QString fileName = state.latestLoadArg;
    int n = fileName.lastIndexOf(".");

    char numberFilled[16];
    sprintf(numberFilled, "%04d", fileCountervalue());

    QString ext = (extension.at(0) == '.') ? extension : ("." + extension);
    return folderPath + "/" + fileName.left(n) + "_" + numberFilled + ext;
}
QDir Environment::makeDir(QDir baseDir, QDir specificDir) {
    QDir dir = locateDir(baseDir, specificDir);
    if (!dir.mkpath("."))
        throw Exception("Could not create folder", dir.path());
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
    return "environment/file-counter/" + state.latestLoadArg;
}

void Environment::copyToClipboard(QString text) {
    QApplication::clipboard()->setText(text);
}

}

