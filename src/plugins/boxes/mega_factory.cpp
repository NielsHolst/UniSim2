#include <typeinfo>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"
#include "object_pool.h"

namespace boxes {

MegaFactory *MegaFactory::_me = 0;
int MegaFactory::productCount = 0;

MegaFactory::MegaFactory() {
    setObjectName("MegaFactory");
    QDir dir = QDir::current();
    dir.cd("plugins");
    foreach (QString filename, dir.entryList(QDir::Files)) {
        QString filePath = dir.absoluteFilePath(filename);
        QPluginLoader loader(filePath);
        FactoryPlugIn *factory = qobject_cast<FactoryPlugIn*>(loader.instance());
        if (factory) {
            _factories << factory;
            for (QString id : factory->inventory()) {
                productIndex[id] = factory;
                QString idWithNamespace = factory->id() + "::" + id;
                productIndex[idWithNamespace] = factory;

            }
        }
    }
}

QString MegaFactory::id() {
    return "MegaFactory";
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
    switch (me()->productIndex.count(className)) {
    case 0:
        throw Exception("No model of class: " + className);
    case 1:
        factory = me()->productIndex[className];
        creation = factory->create(removeNamespace(className), objectName, parent);
        creation->setProperty("classLabel", className);
        break;
    default:
        QString msg = "More than one model of class '%1'. Qualify type with plug-in name";
        throw Exception(msg.arg(className));
    }
    return creation;
}

const QList<FactoryPlugIn*>& MegaFactory::factories() {
    return me()->_factories;
}

MegaFactory* MegaFactory::me() {
    if (!_me)
        _me = objectPool()->find<MegaFactory*>(id());
    return _me;
}

} //namespace
