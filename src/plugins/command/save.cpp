#include <QSet>
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/factory_plug_in.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include "save.h"
#include "save_output.h"

using namespace base;

namespace command {

PUBLISH(save)
HELP(save_grammar, "save grammar", "save grammar for Atom editor")

save::save(QString name, QObject *parent)
    : Command(name, parent)
{
}

void save::doExecute() {
    if (_args.size() != 2 || _args.at(1) != "grammar")
        ThrowException("Write: save grammar");
    openFileStream();
    writeBeginning();
    writePlugins();
    writeClasses();
    writePorts();
    writeEnd();
    closeFileStream();
 }

void save::openFileStream() {
    _file.setFileName(filePath());
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath()).context(this);
    _stream.setDevice(&_file);
}

QString save::filePath() {
    return environment().outputFileNamePath("boxes.cson");;
}


void save::writeBeginning() {
    QFile beginning(":boxes_head.cson");
    if ( !beginning.open(QIODevice::ReadOnly | QIODevice::Text) )
        ThrowException("Cannot open file").context(this);
    _stream
        << beginning.readAll()
        << "\n# Generated by Universal Simulator from here\n";
}

void save::writePlugins() {
    QStringList names;
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command")
            names << factory->id();
    }
    writePattern(names, "support.class");
}

void save::writeClasses() {
    QSet<QString> names;
    names << "Box";
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command")
            names.unite( QSet<QString>::fromList(factory->inventory()) );
    }
    QStringList sorted = names.toList();
    sorted.sort();
    writePattern(sorted, "support.class");
}

void save::writePorts() {
    // Create an object of each class
    Box *root = new Box("root", 0);
    for (FactoryPlugIn *factory : MegaFactory::factories()) {
        if (factory->id() != "command") {
            for (QString className : factory->inventory())
                MegaFactory::create<Box>(className, className.toLower(), root);
        }
    }

    // Find and sort all unique port names
    QVector<Port*> ports = Path("*{Port}", root).resolveMany<Port>();
    QSet<QString> names;
    for (Port *port : ports)
        names << port->objectName();
    QStringList sorted = names.toList();
    sorted.sort();
    writePattern(sorted, "support.variable");

    // Delete all the objects
    root->deleteLater();
}

void save::writePattern(QStringList match, QString name) {
    _stream
        << "  {\n"
        << "    'match': '\\\\b(" << match.join("|") << ")\\\\b'\n"
        << "    'name': '" << name << "'\n"
        << "  }\n";
}

void save::writeEnd() {
    _stream
        << "]\n"
        << "# Generated by Universal Simulator until here\n";
}

void save::closeFileStream() {
    _file.close();
}

}
