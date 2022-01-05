/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <typeinfo>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QSettings>
#include <QStringList>
#include "box.h"
#include "construction_step.h"
#include "dialog.h"
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"
#include "object_pool.h"


namespace base {

MegaFactory *MegaFactory::_me = nullptr;
QString MegaFactory::_usingPluginName;

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
            ThrowException("Unknown class").value(className)
                    .value2("\nUncreated object: "+objectName+"\nof parent: "+fullName(parent));
        case 1:
            factory = me()->productIndex.value(className);
            creation = factory->create(removeNamespace(className), objectName, parent);
            break;
        default:
            // Try again with 'using' plugin name
            QString pluginName = _usingPluginName;
            if (!className.contains("::") && !pluginName.isEmpty())
                creation = createObject(pluginName+"::"+className, objectName, parent);
            else {
                QString msg = "Qualify class name with plug-in name as in:\n" +
                        qualifiedClassNames(className).join("\n");
                ThrowException(msg);
            }
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

void MegaFactory::usingPlugin(QString pluginName) {
    _usingPluginName = pluginName;
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

 QStringList MegaFactory::find(QString className) {
     QStringList names;
     auto factories = me()->productIndex.values(className);
     for (auto factory : factories)
         names << factory->id();
     return names;
 }

} //namespace
