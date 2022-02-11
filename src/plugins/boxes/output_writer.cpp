/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <limits>
#include <QLocale>
#include <base/box_builder.h>
#include <base/convert.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/unique_name.h>
#include "output_r.h"
#include "output_writer.h"

using namespace base;

namespace boxes {

PUBLISH(OutputWriter)

OutputWriter::OutputWriter(QString name, Box *parent)
    : Box(name, parent)
{
    help("writes the output text file");
    Input(ports).computes("/*[iteration] | OutputText::*[ports] | OutputR::*[ports]");
    Input(skipFormats).imports("OutputSelector::*[skipFormats]");
    Input(useLocalDecimalChar).imports("OutputSelector::*[useLocalDecimalChar]");
    Input(isActive).imports("OutputSelector::*[isActive]");
    Output(filePath).noClear().help("Name of output file including absolute path");
}

void OutputWriter::amend() {
    // If it does not exist, create an OutputSelector as a child box
    if (!findMaybeOne<Box*>("OutputSelector::*")) {
        BoxBuilder builder(this);
        builder.
        box("OutputSelector").name("selector").endbox();
        findOne<Box*>("./OutputSelector::selector")->amendFamily(false);    }
}

void OutputWriter::initialize() {
    // Find all ports in output and set their output names
    setColumnNames();

    // Set decimal character
    if (useLocalDecimalChar)
        QLocale::setDefault(QLocale::system());

    // Open output file and write column headings and formats
    openFileStream();
    writeColumnLabels();
    if (!skipFormats)
        writeColumnFormats();

    // If output is for R then add R code to skip format
    if (skipFormats) {
        OutputR *outputR = findMaybeOne<OutputR*>("*");
        if (outputR)
            outputR->addRCode("output_skip_formats = TRUE");
    }
}

inline bool isNumberLike(const Port *port) {
    const Value::Type &type(port->value().type());
    return (type==Value::Type::Bool || type==Value::Type::Int || type==Value::Type::Double);
}

void OutputWriter::setColumnNames() {
    auto portPtrs = ports.findMany<Port*>();
    QStringList uniqueNames = UniqueName(portPtrs).resolved();
    auto name = uniqueNames.begin();
    auto port = portPtrs.begin();
    for (; name != uniqueNames.end(); ++name, ++port)
        (*port)->outputName(*name);
}

void OutputWriter::openFileStream() {
    _file.close();
    environment().openOutputFile(_file, "txt");
    filePath = _file.fileName();
    _stream.setDevice(&_file);
}


void OutputWriter::reset() {
    update();
}

void OutputWriter::update() {
    if (isActive)
        writeValues();
}

void OutputWriter::writeColumnLabels() {
    QStringList list;
    for (auto port : ports.findMany<Port*>())
        list << port->outputName();
    _stream << list.join("\t") << "\n";
}

void OutputWriter::writeColumnFormats() {
    QStringList list;
    auto test = ports.findMany<Port*>();
    for (auto port : test)
        list << port->format();
    _stream << list.join("\t") << "\n";
}

void OutputWriter::writeValues() {
    // To collect values to be written
    QStringList values;
    // Loop through ports
    auto test = ports.findMany<Port*>();
    for (auto port : test)
        values << port->value().asString(true, false);
    _stream << values.join("\t") << "\n";
}

void OutputWriter::debrief() {
    QLocale::setDefault(QLocale::C);
    dialog().information("Data frame written to '" + filePath + "'");
    _file.close();
}

void OutputWriter::cleanup() {
    if (isActive)
        writeValues();
}

}
