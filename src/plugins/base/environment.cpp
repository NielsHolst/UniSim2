#include <QDir>
#include <QSettings>
#include "environment.h"
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

}

