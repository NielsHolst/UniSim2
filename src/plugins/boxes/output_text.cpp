/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <limits>
#include <QLocale>
#include <base/convert.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/unique_name.h>
#include "output_r.h"
#include "output_text.h"

using namespace base;

namespace boxes {

PUBLISH(OutputText)

OutputText::OutputText(QString name, QObject *parent)
    : OutputPorts(name, parent)
{
    help("creates an output text file");
    Input(skipFormats).equals(false).help("Skip line with column formats?");
    Input(useLocalDecimalChar).equals(false).help("Use local decimal character in output?");
    Input(step).imports("/*[step]");
    Input(isActive).imports("./*[isActive] | ../*[isActive]"); // look in child and sibling
    Input(summary) .imports("./*[summary]  | ../*[summary]");  // look in child and sibling
    Output(filePath).noReset().help("Name of output file including absolute path");
}

void OutputText::initialize() {
    // Find all ports in output and set their output names
    collectPaths();
    setOutputNames();

    // Reset summaries
    _summary = OutputSummary(summary, this);
    initializeState();

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
        OutputR *outputR = findMaybeOne<OutputR>("*");
        if (outputR)
            outputR->addRCode("output_skip_formats = TRUE");
    }
}
void OutputText::collectPaths() {
    // Always output Simulation iteration
    ports = Path(ports.original() + "|/*[iteration]");

    // Collect ports if this is a child of an OutputR box
    bool doCollect = findMaybeOne<Box>("../*<OutputR>");
    if (doCollect) {
        QStringList paths;
        paths << ports.original();
        QVector<Box*> pages = findMany<Box>("../*<PageR>");
        for (auto page : pages) {
            paths << page->port("xAxis")->value<Path>().original();
            QVector<Box*> plots = page->findMany<Box>("../*<PlotR>");
            for (auto plot : plots) {
                paths << plot->port("ports")->value<Path>().original();
            }
        }
        ports = Path(paths.join("|"));
    }
}

inline bool isNumberLike(const Port *port) {
    const Value::Type &type(port->value().type());
    return (type==Value::Type::Bool || type==Value::Type::Int || type==Value::Type::Double);
}

void OutputText::setOutputNames() {
    auto portPtrs = ports.resolved();
    QStringList names = UniqueName(portPtrs).resolved();
    auto name = names.begin();
    auto port = portPtrs.begin();
    for (; name != names.end(); ++name, ++port) {
        QStringList colNamesForThisPort;
        using Type = OutputSummary::Type;
        if (_summary.size() <= 1)                   colNamesForThisPort <<  *name;
        else if (isNumberLike(*port)) {
            if (_summary.contains(Type::Current))   colNamesForThisPort << (*name + ".final"); // current summary means final
            if (_summary.contains(Type::Sum))       colNamesForThisPort << (*name + ".sum");
            if (_summary.contains(Type::Mean))      colNamesForThisPort << (*name + ".mean");
            if (_summary.contains(Type::MinAt))     colNamesForThisPort << (*name + ".minAt");
            if (_summary.contains(Type::MaxAt))     colNamesForThisPort << (*name + ".maxAt");
            if (_summary.contains(Type::Min))       colNamesForThisPort << (*name + ".min");
            if (_summary.contains(Type::Max))       colNamesForThisPort << (*name + ".max");
        }
        (*port)->outputNames(colNamesForThisPort);
    }
}

void OutputText::openFileStream() {
    _file.close();
    environment().openOutputFile(_file, "txt");
    filePath = _file.fileName();
    _stream.setDevice(&_file);
}

void OutputText::initializeState() {
    using Type = OutputSummary::Type;
    int n = ports.resolved().size();
    if (_summary.contains(Type::Sum) || _summary.contains(Type::Mean)) {
        _state.sum.fill(0., n);
    }
    if (_summary.contains(Type::Min)) {
        _state.min.fill(std::numeric_limits<double>::max(), n);
    }
    if (_summary.contains(Type::Max)) {
        _state.max.fill(std::numeric_limits<double>::lowest(), n);
    }
    if (_summary.contains(Type::MinAt)) {
        _state.minAt.fill(0, n);
    }
    if (_summary.contains(Type::MaxAt)) {
        _state.maxAt.fill(0, n);
    }
    _periodCount = 0;
}

void OutputText::reset() {
    _justChangedToActive = false;
    initializeState();
    update();
}

void OutputText::update() {
    if (isActive)
        writeValues();
    else
        updateState();

    _justChangedToActive = isActive && !_justChangedToActive;
    if (_justChangedToActive)
        initializeState();
}

void OutputText::updateState() {
    using Type = OutputSummary::Type;
    int i=0;
    for (auto port : ports.resolved()) {
        if (isNumberLike(port)) {
            if (_summary.contains(Type::Sum) || _summary.contains(Type::Mean))
                _state.sum[i] += port->value<double>();
            if (_summary.contains(Type::MinAt) && _state.min.at(i) > port->value<double>())
                _state.minAt[i] = step;
            if (_summary.contains(Type::MaxAt) && _state.max.at(i) < port->value<double>())
                _state.maxAt[i] = step;
            if (_summary.contains(Type::Min))
                _state.min[i] = std::min(_state.min.at(i), port->value<double>());
            if (_summary.contains(Type::Max))
                _state.max[i] = std::max(_state.max.at(i), port->value<double>());
        }
        ++i;
    }
    ++_periodCount;
}

void OutputText::writeColumnLabels() {
    QStringList list;
    for (auto port : ports.resolved())
        list << port->outputNames();
    _stream << list.join("\t") << "\n";
}

void OutputText::writeColumnFormats() {
    QStringList list;
    for (auto port : ports.resolved()) {
        if (_summary.size() <= 1)
            list << port->format();
        else if (isNumberLike(port)) {
            for (int i=0; i<_summary.size(); ++i)
                list << port->format();
        }
    }
    _stream << list.join("\t") << "\n";
}

void OutputText::writeValues() {
    // To collect values to be written
    QStringList values;
    // Loop through ports
    int i=0;
    for (auto port : ports.resolved()) {
        using Type = OutputSummary::Type;
        if (_summary.size() <= 1)               values << port->value<QString>();
        else if (isNumberLike(port)) {
            if (_summary.contains(Type::Sum))   values << convert<QString>(_state.sum.at(i));
            if (_summary.contains(Type::Mean))  values << convert<QString>(_state.sum.at(i)/_periodCount);
            if (_summary.contains(Type::MinAt)) values << convert<QString>(_state.minAt.at(i));
            if (_summary.contains(Type::MaxAt)) values << convert<QString>(_state.maxAt.at(i));
            if (_summary.contains(Type::Min))   values << convert<QString>(_state.min.at(i));
            if (_summary.contains(Type::Max))   values << convert<QString>(_state.max.at(i));
        }
        ++i;
    }
    _stream << values.join("\t") << "\n";
}

void OutputText::debrief() {
    QLocale::setDefault(QLocale::C);
    dialog().information("Data frame written to '" + filePath + "'");
    _file.close();
}

void OutputText::cleanup() {
    if (isActive)
        writeValues();
}

}
