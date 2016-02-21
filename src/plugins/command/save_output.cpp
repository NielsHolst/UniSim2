#include <stdio.h>
#include <QDateTime>
#include <QSettings>
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/unique_name.h>
#include "save_output.h"

using namespace base;

namespace command {

PUBLISH(save_output)
HELP(save_output, "save output", "writes all tracked ports to output file")

save_output::save_output(QString name, QObject *parent)
    : Command(name, parent)
{
    Class(save_output);
}

void save_output::doExecute() {
    Box *root = environment().state.root;
    if (_args.size() > 2) {
        throw Exception("Command 'save output' takes no arguments");
    }
    else if (root) {
        collectPortsWithTrack();
        writeOutput();
        writeScript();
        updateOutputNumber();
    }
    else
        throw Exception("No box loaded");
}

void save_output::collectPortsWithTrack() {
    _ports.clear();
    Path path("*{Port}", this);
    QVector<Port*> all = path.resolve<Port>(-1, this);
    for (Port *port : all) {
        if (port->trackPtr())
            _ports << port;
    }
}

void save_output::writeOutput() {
    QString filePath = outputFilePath();
    openFile(filePath);
    DialogBase &dia(dialog());
    dia.information(QString("Writing output to '%1'").arg(filePath));

    QTextStream os(&_file);
    writeTracks(os);
    _file.close();
    environment().state.latestOutputFilePath = filePath;
}

void save_output::writeScript() {
    QString filePath = scriptFilePath();
    openFile(filePath);
    dialog().information(QString("Writing script to '%1'").arg(filePath));

    QTextStream script(&_file);
    writeScriptCode(script);
    _file.close();
}

void save_output::openFile(QString filePath) {
    _file.setFileName(filePath);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        throw Exception(QString("Could not open file"), filePath);
}

QString save_output::outputFilePath() {
    char numberFilled[16];
    sprintf(numberFilled, "%04d", getOutputNumber());

    QDir dir = makeDir(environment().state.dir.work, environment().state.dir.output);

    QString filePath = dir.absolutePath() + "/" + fileName() + "_" + numberFilled + ".txt";
    filePath.replace("\\", "/");
    return filePath;
}

QString save_output::fileName() {
    QString fileName = environment().state.root->objectName();
    return fileName.isEmpty() ? className(environment().state.root) : fileName;
}

QString save_output::scriptFilePath() {
    QString filePath = scriptFolderPath() + "/" + fileName() + ".R";
    filePath.replace("\\", "/");
    return filePath;
}

QString save_output::scriptFolderPath() {
    QDir dir = makeDir(environment().state.dir.work, environment().state.dir.script);
    QString folderPath = dir.absolutePath();
    folderPath.replace("\\", "/");
    return folderPath;
}

void save_output::writeTracks(QTextStream &stream) {

    // Check ports
    if (_ports.isEmpty())
        throw Exception("No ports are being tracked");

    // Check number of rows
    int nrow = _ports.at(0)->trackPtr()->size();
    for (Port *port : _ports) {
        Q_ASSERT(port->trackPtr()->size() == nrow);
    }
    Port *last = _ports.last();

    // Write columns labels
    writeColumnLabels(stream);

    // Write columns format
    for (Port *port : _ports) {
        stream << port->Rformat();
        if (port != last)
            stream << "\t";
    }
    stream << "\n";

    // Write axis
    for (Port *port : _ports) {
        stream << port->axis();
        if (port != last)
            stream << "\t";
    }
    stream << "\n";

    // Write page
    for (Port *port : _ports) {
        stream << port->page();
        if (port != last)
            stream << "\t";
    }
    stream << "\n";

    // Write columns values as text
    for (int row = 0; row < nrow; ++row) {
        for (Port *port : _ports) {
            stream << port->trackPtr()->toString(row);
            if (port != last)
                stream << "\t";
        }
        if (row < nrow-1)
            stream << "\n";
    }
}

void save_output::writeColumnLabels(QTextStream &stream) {
//    QStringList labels;
//    for (Port *port : _ports)
//        labels << port->label();
    UniqueName unique(_ports);
    QStringList labels = unique.resolve();
    stream << labels.join("\t") << "\n";
}

void save_output::writeScriptCode(QTextStream &stream) {
    stream << "# Generated on " << QDateTime::currentDateTime().toString() << "\n";
    stream << "output_file_path <- \"" << outputFilePath() << "\"\n";
    stream << "script_folder_path <- \"" << scriptFolderPath() << "\"\n";
    stream << "setwd(script_folder_path)\n";
    stream << "source(\"common.R\")\n";
}

QString save_output::keyOutputNumber() {
    return "output/" + save_output::fileName();
}

int save_output::getOutputNumber() {
    QSettings settings;
    bool ok;
    int number = settings.value(keyOutputNumber(), 0).toInt(&ok);
    return ok ? number : 0;
}

void save_output::updateOutputNumber() {
    QSettings settings;
    int number = getOutputNumber();
    settings.setValue(keyOutputNumber(), ++number);
}

}
