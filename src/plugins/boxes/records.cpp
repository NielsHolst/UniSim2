#include <iostream>
#include <QtAlgorithms>
#include <QDir>
#include <QTime>
#include <base/convert.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/publish.h>
#include "records.h"

using namespace base;

namespace boxes {

PUBLISH(Records)

Records::Records(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(fileName).equals("records.txt");
    Input(calendarDateTime).imports("calendar[dateTime]");

    Output(currentDateTime).trackOff();
    Output(nextDateTime).trackOff();
    Output(firstDateTime).trackOff();
    Output(lastDateTime).trackOff();
    Output(currentDate).trackOff();
    Output(nextDate).trackOff();
    Output(currentTime).trackOff();
    Output(nextTime).trackOff();

    currentColumnValues = new QVector<double>;
    nextColumnValues = new QVector<double>;
}

void Records::amend() {
    readColumnNames();
    createColumnOutputs();
    readFromFirstToLastLine();
}

Records::~Records() {
    delete currentColumnValues;
    delete nextColumnValues;
}

void Records::readColumnNames() {
    openFile();
    readLineItems();
    if (pastLastLine)
        throw Exception("Records file is empty", filePath(), this);

    dateColumn = -1;
    timeColumn = -1;
    for (int i = 0; i < lineItems.size(); ++i) {
        QString id = lineItems[i];
        columnNames.append(id);
        if (id.toLower()  == "date")
            dateColumn = i;
        else if (id.toLower() == "time")
            timeColumn = i;
    }
    file.close();
}

void Records::openFile() {
    file.setFileName(filePath());
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        throw Exception("Cannot open records file", filePath(), this);
    pastLastLine = false;
}

QString Records::filePath() {
    QDir dir = locateDir(environment().state.dir.work, environment().state.dir.input);
    if (!dir.exists())
        throw Exception("Input folder not found", dir.absolutePath());
    return dir.absoluteFilePath(fileName);
}

void Records::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine().simplified());
    }
    lineItems = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    pastLastLine = lineItems.isEmpty();
}

#define NamedOutput(X,Y) (*new Port(X, this)).data(& Y).access(Port::Read).zeroAtReset().trackOn()

void Records::createColumnOutputs() {
    int n = columnNames.size();
    values.fill(0., n);
    currentColumnValues->fill(0., n);
    nextColumnValues->fill(0., n);
    for (int i = 0; i < n; ++i) {
        QString name = columnNames[i];
        if (i != dateColumn && i != timeColumn) {
            Port &port( NamedOutput(name, values[i]) );
            // Hack! In stead provide list(!) of column names to ignore as input
            if (name.toLower() == "station")
                port.trackOff();
        }
    }
}

void Records::readFromFirstToLastLine() {
    openFile();
    readLineItems(); // skip labels
    advanceFirstLine();
    firstDateTime = currentDateTime;
    while (!pastLastLine)
        advanceLine();
    lastDateTime = currentDateTime;
    file.close();
}

void Records::reset() {
    readToFirstLine();
    update();
}

void Records::readToFirstLine() {
    currentDate = nextDate = QDate(2000,1,1);
    currentTime = nextTime = QTime(0,0,0);
    currentDateTime = nextDateTime = QDateTime(currentDate, currentTime, Qt::UTC);

    openFile();
    readLineItems(); // skip labels
    advanceFirstLine();
}

void Records::advanceFirstLine() {
    readLineItems();
    if (pastLastLine)
        throw Exception("Records file is empty", filePath(), this);
    extractValues();
    advanceTime();
    for (int i = 0; i < nextColumnValues->size(); ++i)
        (*currentColumnValues)[i] = nextColumnValues->at(i);

}

void Records::advanceTime() {
    currentDate = nextDate;
    currentTime = nextTime;
    currentDateTime = nextDateTime;

}

void Records::extractValues() {
    if (!pastLastLine && lineItems.size() != columnNames.size())
        throw Exception("Number of items in records file does not match number of column names",
                        lineItems.join(" "), this);

    for (int i = 0; i < lineItems.size(); ++i) {
        if (i == dateColumn)
            nextDate = convert<QDate>(lineItems[dateColumn]);
        else if (i == timeColumn)
            nextTime = convert<QTime>(lineItems[timeColumn]);
        else
            (*nextColumnValues)[i] = convert<double>(lineItems[i]);
    }
    nextDateTime = QDateTime(nextDate, nextTime, Qt::UTC);
}

void Records::advanceLine() {
    advanceTime();
    qSwap(currentColumnValues, nextColumnValues);
    readLineItems();

    if (!pastLastLine) {
        extractValues();
    }
    else {
        for (int i = 0; i < nextColumnValues->size(); ++i)
            (*nextColumnValues)[i] = currentColumnValues->at(i);
    }
}

void Records::update() {
    while (calendarDateTime > nextDateTime && !pastLastLine)
        advanceLine();

    for (int i = 0; i < values.size(); ++i) {
        if (i == dateColumn || i == timeColumn) continue;
        double dx = currentDateTime.secsTo(nextDateTime);
        double dy = nextColumnValues->at(i) - currentColumnValues->at(i);
        double x = currentDateTime.secsTo(calendarDateTime);
        values[i] = ((dx > 0) ? x*dy/dx : 0.) + currentColumnValues->at(i);
    }
}

void Records::cleanup() {
    file.close();
}


} //namespace
