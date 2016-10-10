#include <iostream>
#include <QtAlgorithms>
#include <QDir>
#include <QTime>
#include <base/convert.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include "scenarios.h"

using namespace base;

namespace boxes {

PUBLISH(Scenarios)

Scenarios::Scenarios(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(fileName).equals("scenarios.txt");
    Input(separator).equals("whitespace");
    Output(atEnd);
}

void Scenarios::amend() {
    readColumnNames();
    createColumnOutputs();
    atEnd = false;
    checkSeparator();
    openFile();
    readLineItems();
    readLineItems();
    if (atEnd)
        ThrowException("No scenarios in file").value(fileName).context(this);
    updateValues();
}

void Scenarios::initialize() {
}

void Scenarios::checkSeparator() {
    if (separator != "whitespace" && separator != "tab")
        ThrowException("Scenarios separator must be either 'whitespace' or 'tab'").value(separator).context(this);
}

void Scenarios::readColumnNames() {
    openFile();
    readLineItems();
    if (pastLastLine)
        ThrowException("Records file is empty").value(fileName).context(this);
    columnNames = lineItems;
    file.close();
}

void Scenarios::openFile() {
    QString fileNamePath = environment().inputFileNamePath(fileName);
    file.setFileName(fileNamePath);
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        ThrowException("Cannot open scenarios file").value(fileNamePath).context(this);
    pastLastLine = false;
}

void Scenarios::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine().simplified());
    }
    lineItems = (separator == "tab") ?
                line.split("\t", QString::KeepEmptyParts) :
                line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    atEnd = lineItems.isEmpty();
}

void Scenarios::createColumnOutputs() {
    int n = columnNames.size();
    values.fill(QString(), n);
    for (int i = 0; i < n; ++i) {
        QString name = columnNames[i];
        Port *port = new Port(columnNames.at(i), this);
        port->data(&values[i]);
    }
}

void Scenarios::reset() {
    if (atEnd)
        ThrowException("Read past end of file in scenarios").value(fileName).context(this);
    updateValues();
    readLineItems();
}

void Scenarios::updateValues() {
    int n = lineItems.size();
    if (n != values.size()) {
        QString msg{"Wrong number of items in line. Expected %1 got %2"};
        ThrowException(msg.arg(values.size()).arg(n)).value("\n"+lineItems.join("\n")).context(this);
    }
    for (int i = 0; i < n; ++i)
        values[i] = lineItems.at(i);
}

void Scenarios::debrief() {
    file.close();
}


} //namespace
