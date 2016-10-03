#include <typeinfo>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QSettings>
#include "box.h"
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
    QDir dir(QApplication::applicationDirPath());
    bool gotoPlugins = dir.cd("plugins");
//    QDir dir("/Users/nielsholst/qdev/UniSim2/bin/plugins");
//    dialog().information("Dir = "+ dir.absolutePath());
    if (!gotoPlugins)
        dialog().error("Could not find plugins folder,\nexpected here: " + dir.absolutePath()+"/plugins");
    foreach (QString filename, dir.entryList(QDir::Files)) {
        QString filePath = dir.absoluteFilePath(filename);
        QPluginLoader loader(filePath);
        FactoryPlugIn *factory = qobject_cast<FactoryPlugIn*>(loader.instance());
        if (factory) {
            dialog().information("loading " + filePath +"...");
            _factories << factory;
            for (QString id : factory->inventory()) {
                productIndex.insert(id, factory);
                QString idWithNamespace = factory->id() + "::" + id;
                productIndex.insert(idWithNamespace, factory);

            }
        }
    }
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
        return creation;
    }
    switch (me()->productIndex.count(className)) {
    case 0:
        ThrowException("Unknown class").value(className);
    case 1:
        factory = me()->productIndex.value(className);
        creation = factory->create(removeNamespace(className), objectName, parent);
        break;
    default:
        QString msg = "More than one class is named '%1'. Qualify class name with plug-in name";
        ThrowException(msg.arg(className));
    }
    return creation;
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
