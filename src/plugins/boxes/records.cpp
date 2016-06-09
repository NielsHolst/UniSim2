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
    Input(dateColumnName).equals("Date");
    Input(timeColumnName).equals("Time");
    Input(calendarDateTime).imports("calendar[dateTime]");

    Output(currentDateTime);
    Output(nextDateTime);
    Output(firstDateTime);
    Output(lastDateTime);
    Output(currentDate);
    Output(nextDate);
    Output(currentTime);
    Output(nextTime);

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
        ThrowException("Records file is empty").value(filePath()).context(this);

    dateColumn = -1;
    timeColumn = -1;
    for (int i = 0; i < lineItems.size(); ++i) {
        QString id = lineItems[i];
        columnNames.append(id);
        if (id == dateColumnName)
            dateColumn = i;
        else if (id == timeColumnName)
            timeColumn = i;
    }
    file.close();
}

void Records::openFile() {
    file.setFileName(filePath());
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        ThrowException("Cannot open records file").value(filePath()).context(this);
    pastLastLine = false;
}

QString Records::filePath() {
    return environment().filePath(Environment::Input, fileName);
}

void Records::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine().simplified());
    }
    lineItems = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    pastLastLine = lineItems.isEmpty();
}

#define NamedOutput(X,Y) (*new Port(X, this)).data(& Y).zeroAtReset()

void Records::createColumnOutputs() {
    int n = columnNames.size();
    values.fill(0., n);
    currentColumnValues->fill(0., n);
    nextColumnValues->fill(0., n);
    for (int i = 0; i < n; ++i) {
        QString name = columnNames[i];
        if (i != dateColumn && i != timeColumn) {
            Port &port( NamedOutput(name, values[i]) );
            port.page("default");
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
        ThrowException("Records file is empty").value(filePath()).context(this);
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
        ThrowException("Number of items in records file does not match number of column names").
                        value(lineItems.join(" ")).context(this);

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
