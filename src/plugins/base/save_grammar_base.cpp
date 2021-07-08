/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"
#include "port.h"
#include "port_filter.h"
#include "port_transform.h"
#include "save_grammar_base.h"

namespace base {

SaveGrammarBase::SaveGrammarBase() {
}

void SaveGrammarBase::write() {
    openFileStream();
    writeBeginning();
    writePlugins();
    writeClasses();
    writePorts();
    writeAttributes();
    writeTransforms();
    writeConstants();
    writeEnd();
    closeFileStream();
}


void SaveGrammarBase::openFileStream() {
    _file.setFileName(grammarFilePath());
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(grammarFilePath());
    _stream.setDevice(&_file);
}

void SaveGrammarBase::closeFileStream() {
    dialog().information("grammar saved to " + grammarFilePath() + "...");
    _file.close();
}

QStringList SaveGrammarBase::plugInNames() {
    QStringList names;
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command")
            names << factory->id();
    }
    return names;
}

QStringList SaveGrammarBase::classNames() {
    QSet<QString> names;
    names << "Box";
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command") {
            auto inv = factory->inventory();
            #if QT_VERSION >= 0x050E00
                names.unite(QSet<QString>(inv.begin(), inv.end()));
            #else
                names.unite(QSet<QString>(inv.toSet()));
            #endif
        }
    }
    QStringList sorted = names.values();
    sorted.sort();
    return sorted;
}

QStringList SaveGrammarBase::portNames() {
    // Collect port names of current root
    QSet<QString> names = collectPortNames(environment().root());

    // Create an object of each class
    Box *root = new Box("root", nullptr);
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command") {
            for (QString className : factory->inventory()) {
                QString qualifiedClassName = factory->id()+ "::" + className;
                try {
                    MegaFactory::create<Box>(qualifiedClassName, className.toLower(), root);
                }
                catch (Exception &ex) {
                    dialog().information("Class not included in grammar (error in its constructor): " + qualifiedClassName);
                    dialog().error(ex.what());
                }
            }
        }
    }
    // Pool port names
    names |= collectPortNames(root);

    // Delete all the objects
    root->deleteLater();

    // Sort port names
    QStringList sorted = QStringList( QList<QString>(names.begin(), names.end()) );
    sorted.sort();
    return sorted;
}

QSet<QString> SaveGrammarBase::collectPortNames(const Box *root) {
    QSet<QString> names;
    if (root) {
        QVector<Port*> ports = Path("*<Port>", root).resolveMany<Port>();
        for (Port *port : ports)
            names << port->objectName();
    }
    return names;
}

QStringList SaveGrammarBase::attributeNames() {
    return QStringList(); // unused
}

QStringList SaveGrammarBase::transformNames() {
    QStringList tNames = portTransformNames(),
                fNames = portFilterNames();
    QSet<QString> names;
    #if QT_VERSION >= 0x050E00
        names.unite(QSet<QString>(tNames.begin(), tNames.end()));
        names.unite(QSet<QString>(fNames.begin(), fNames.end()));
    #else
        names.unite(QSet<QString>(tNames.toSet()));
        names.unite(QSet<QString>(fNames.toSet()));
    #endif
    return QStringList(names.begin(), names.end());
}

QStringList SaveGrammarBase::constantNames() {
    return QStringList() << "FALSE" << "TRUE";
}

}
