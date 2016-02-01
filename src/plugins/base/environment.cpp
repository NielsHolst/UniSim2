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
    setDir();
    setOutputFolder();
    state.root = 0;
}

Environment::~Environment() {
    QSettings settings;
    settings.setValue("environment/working-directory", state.dir.absolutePath());
    settings.setValue("environment/output-folder", state.outputFolder);
}

void Environment::setDir() {
    QSettings settings;
    QString path = settings.value("environment/working-directory", "~").toString();
    if (path.isEmpty())
        state.dir = QDir::home();
    else {
        state.dir = QDir(path);
        if (!state.dir.exists())
            state.dir = QDir::home();
    }
}

void Environment::setOutputFolder() {
    QSettings settings;
    state.outputFolder = settings.value("environment/output-folder", QString("output")).toString();
//    QDir output(state.outputFolder),
//         path;
//    if (output.isAbsolute())
//        path = output;
//    else {
//        path = dir;
//        if (!path.mkpath(output)) {
//            path.mkpath( state.outputFolder = "output" );
//        }
//    }
}

}

