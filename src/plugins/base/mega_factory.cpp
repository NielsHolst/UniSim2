#include <typeinfo>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QSettings>
#include "box.h"
#include "construction_step.h"
#include "dialog.h"
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"
#include "object_pool.h"


namespace base {

MegaFactory *MegaFactory::_me = 0;
int MegaFactory::productCount = 0;

MegaFactory::MegaFactory() {
    setObjectName("MegaFactory");
    QDir dir = pluginsDir();
    bool notFound = (dir == QDir("/"));
    if (notFound)
        dialog().error("Could not find plugins folder,\nexpected near here: " +
                       QDir(QApplication::applicationDirPath()).absolutePath());

    dialog().pushMessage();
    foreach (QString filename, dir.entryList(QDir::Files)) {
        QString filePath = dir.absoluteFilePath(filename);
        QPluginLoader loader(filePath);
        FactoryPlugIn *factory = qobject_cast<FactoryPlugIn*>(loader.instance());
        if (factory) {
            dialog().message("loading " + filePath +"...");
            _factories << factory;
            for (QString id : factory->inventory()) {
                productIndex.insert(id, factory);
                QString idWithNamespace = factory->id() + "::" + id;
                productIndex.insert(idWithNamespace, factory);
            }
        }
    }
    dialog().popMessage();
}

QDir MegaFactory::pluginsDir() {
    QDir dir(QApplication::applicationDirPath());
    bool found;
    do {
        found = dir.cd("plugins");
        if (found) break;
        dir.cdUp();
    } while (dir != QDir("/"));
    return dir;
}

void MegaFactory::loadPlugins() {
    me();
}

namespace {
    QString removeNamespace(QString s) {
        int i = s.lastIndexOf("::");
        int n = s.size() - i - 2;
        return (i == -1)  ? s : s.right(n);
    }
}

QObject* MegaFactory::createObject(QString className, QString objectName, QObject *parent)
{
    FactoryPlugIn *factory;
    QObject *creation;
    if (className == "Box") {
        creation = new Box(objectName, parent);
        setClassName(creation, "Box");
    }
    else {
        switch (me()->productIndex.count(className)) {
        case 0:
            dialog().information("Unknown class: " + className);
            ThrowException("Unknown class").value(className);
        case 1:
            factory = me()->productIndex.value(className);
            creation = factory->create(removeNamespace(className), objectName, parent);
            break;
        default:
            QString msg = "Qualify class name with plug-in name as in:\n" + qualifiedClassNames(className).join("\n");
            ThrowException(msg);
        }
    }

    ConstructionStep *step = dynamic_cast<ConstructionStep*>(creation);
    if (step)
        step->finishConstruction();

    return creation;
}

QStringList MegaFactory::qualifiedClassNames(QString className) {
    QStringList result;
    if (className.contains("::"))
        result << className;
    else {
        QList<FactoryPlugIn*> factories = me()->productIndex.values(className);
        for (auto factory : factories)
            result << (factory->id() + "::" + className);
    }
    return result;
}

const QList<FactoryPlugIn*>& MegaFactory::factories() {
    return me()->_factories;
}

MegaFactory* MegaFactory::me() {
    if (!_me) {
        _me = new MegaFactory;
        objectPool()->attach("MegaFactory", _me);
    }
    return _me;
}

} //namespace
