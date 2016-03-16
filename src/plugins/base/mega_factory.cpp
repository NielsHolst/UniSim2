#include <typeinfo>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>
#include "box.h"
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"
#include "object_pool.h"

//#include "dialog.h"

namespace base {

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
//            dialog().information(filePath);
            _factories << factory;
            for (QString id : factory->inventory()) {
                productIndex[id] = factory;
                QString idWithNamespace = factory->id() + "::" + id;
                productIndex[idWithNamespace] = factory;
//                dialog().information(id + " " + idWithNamespace);

            }
        }
    }
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
        factory = me()->productIndex[className];
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
