#include "box.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "factory_plug_in.h"
#include "mega_factory.h"
#include "port.h"
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
    dialog().information("saving grammar to " + grammarFilePath() + "...");
    _file.setFileName(grammarFilePath());
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(grammarFilePath());
    _stream.setDevice(&_file);
}

void SaveGrammarBase::closeFileStream() {
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
        if (factory->id() != "command")
            names.unite( QSet<QString>::fromList(factory->inventory()) );
    }
    QStringList sorted = names.toList();
    sorted.sort();
    return sorted;
}

QStringList SaveGrammarBase::portNames() {
    // Collect port names of current root
    QSet<QString> names = collectPortNames(environment().root());

    // Create an object of each class
    Box *root = new Box("root", 0);
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command") {
            for (QString className : factory->inventory()) {
                QString qualifiedClassName = factory->id()+ "::" + className;
                MegaFactory::create<Box>(qualifiedClassName, className.toLower(), root);
            }
        }
    }
    // Pool port names
    names |= collectPortNames(root);

    // Delete all the objects
    root->deleteLater();

    // Sort port names
    QStringList sorted = names.toList();
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
    return Port::attributes();
}

QStringList SaveGrammarBase::transformNames() {
    return portTransformNames();
}

QStringList SaveGrammarBase::constantNames() {
    return QStringList() << "FALSE" << "TRUE";
}

}
