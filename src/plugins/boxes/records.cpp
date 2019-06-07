#include <iostream>
#include <QtAlgorithms>
#include <QDir>
#include <QTime>
#include <base/convert.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/port.h>
#include <base/port_access.h>
#include <base/publish.h>
#include "records.h"

using namespace base;

namespace boxes {

PUBLISH(Records)

Records::Records(QString name, QObject *parent)
    : Box(name, parent)
{
    help("reads inputs from records in a text file");
    Input(fileName).help("Name of file with records; columns separated by white space").equals("records.txt");
    Input(dateColumnName).help("Name of column with date").equals("Date");
    Input(timeColumnName).help("Name of column with time").equals("Time");
    Input(cycle).equals(false).help("Cycle back to start at end of file? File must begin on 1 January and end on 31 December");
    Input(calendarDateTime).imports("calendar[dateTime]");
    Input(overrideCalendarYear).equals(false).help("Let first date in records determine calendar year?");

    Output(currentDateTime).help("Date-time stamp of the current outputs");
    Output(nextDateTime).help("Date-time stamp of the next outputs");
    Output(firstDateTime).help("Date-time stamp of the first line in the file");
    Output(lastDateTime).help("Date-time stamp of the last line in the file");
    Output(currentDate).help("Date stamp of the current outputs");
    Output(nextDate).help("Date stamp of the next outputs");
    Output(currentTime).help("Time stamp of the current outputs");
    Output(nextTime).help("Time stamp of the next outputs");;

    _currentColumnValues = new QVector<double>;
    _nextColumnValues = new QVector<double>;
}

void Records::amend() {
    ExceptionContext(this);
    // Allow imports defined in amend step
    resolvePortImports();
    updateImports();
    // Create outputs
    readColumnNames();
    createColumnOutputs();
}

Records::~Records() {
    delete _currentColumnValues;
    delete _nextColumnValues;
}

void Records::readColumnNames() {
    openFile();
    readLineItems();
    if (_pastLastLine)
        ThrowException("Records file is empty").value(fileNamePath()).context(this);

    _dateColumn = -1;
    _timeColumn = -1;
    _columnNames.clear();
    for (int i = 0; i < _lineItems.size(); ++i) {
        QString id = _lineItems[i];
        _columnNames.append(id);
        if (id == dateColumnName)
            _dateColumn = i;
        else if (id == timeColumnName)
            _timeColumn = i;
    }
    _file.close();
}

void Records::openFile() {
    if (_file.isOpen())
        _file.close();
    _file.setFileName(fileNamePath());
    bool fileOk = _file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        ThrowException("Cannot open records file").value(fileNamePath()).context(this);
    _pastLastLine = false;
}

QString Records::fileNamePath() {
    return environment().inputFileNamePath(fileName);
}

void Records::readLineItems() {
    QString line;
    while (!_file.atEnd() && line.isEmpty()) {
        line = QString(_file.readLine().simplified());
    }
    _lineItems = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    _pastLastLine = _lineItems.isEmpty();
}

void Records::createColumnOutputs() {
    int n = _columnNames.size();
    values.fill(0., n);
    _currentColumnValues->fill(0., n);
    _nextColumnValues->fill(0., n);
    for (int i = 0; i < n; ++i) {
        QString name = _columnNames[i];
        if (i != _dateColumn && i != _timeColumn)
            NamedOutput(name, values[i]);
    }
}

void Records::initialize() {
    ExceptionContext(this);
    // Set lastDateTime
    bool saveCycle = cycle;
    cycle = false;
    openFile();
    readLineItems(); // skip labels
    advanceFirstLine();
    firstDateTime = currentDateTime;
//    dialog().information("firstDateTime    A "   + convert<QString>(firstDateTime) +
//                         "\ncurrentDateTime  A " + convert<QString>(currentDateTime) +
//                         "\ncalendarDateTime A " + convert<QString>(calendarDateTime));
    while (!_pastLastLine)
        advanceLine();
    lastDateTime = currentDateTime;
    _file.close();
    cycle = saveCycle;
    doOverrideCalendarYear();
}

void Records::reset() {
    ExceptionContext(this);
    initialize();
    readToFirstLine();
    _yearOffset = calendarDateTime.date().year() - currentDate.year();
    _extraYearOffset = 0;
    update();
}

void Records::readToFirstLine() {
    currentDateTime = firstDateTime;
    currentDate = nextDate = firstDateTime.date();
    currentTime = nextTime = firstDateTime.time();
    nextDateTime = currentDateTime;

    openFile();
    readLineItems(); // skip labels
    advanceFirstLine();
}

void Records::advanceFirstLine() {
    readLineItems();
    if (_pastLastLine)
        ThrowException("Records file is empty").value(fileNamePath()).context(this);
    extractValues();
    advanceTime();
    for (int i = 0; i < _nextColumnValues->size(); ++i)
        (*_currentColumnValues)[i] = _nextColumnValues->at(i);

}

void Records::advanceTime() {
    currentDate = nextDate;
    currentTime = nextTime;
    currentDateTime = nextDateTime;
}

void Records::extractValues() {
    if (!_pastLastLine && _lineItems.size() != _columnNames.size())
        ThrowException("Number of items in records file does not match number of column names").
                        value(_lineItems.join(" ")).context(this);

    try {
        for (int i = 0; i < _lineItems.size(); ++i) {
            if (i == _dateColumn)
                nextDate = convert<QDate>(_lineItems[_dateColumn]);
            else if (i == _timeColumn)
                nextTime = convert<QTime>(_lineItems[_timeColumn]);
            else
                (*_nextColumnValues)[i] = convert<double>(_lineItems[i]);
        }
        nextDateTime = QDateTime(nextDate, nextTime, Qt::UTC);
    }
    catch (Exception &ex) {
        ThrowException(ex.what()).context(this);
    }
}

void Records::extrapolateValues() {
    for (int i = 0; i < _nextColumnValues->size(); ++i)
        (*_nextColumnValues)[i] = _currentColumnValues->at(i);
}

void Records::update() {
    ExceptionContext(this);
    QDateTime currentT, nextT;
    while (true) {
        currentT = alignDateTime(currentDateTime, _yearOffset);
        nextT = alignDateTime(nextDateTime, _yearOffset + _extraYearOffset);
        if (nextT >= calendarDateTime || _pastLastLine)
            break;
        advanceLine();
    }

    for (int i = 0; i < values.size(); ++i) {
        if (i == _dateColumn || i == _timeColumn) continue;
        double dx = currentT.secsTo(nextT);
        double dy = _nextColumnValues->at(i) - _currentColumnValues->at(i);
        double x = currentT.secsTo(calendarDateTime);
        values[i] = ((dx > 0) ? x*dy/dx : 0.) + _currentColumnValues->at(i);
    }
}

void Records::doOverrideCalendarYear() {
    if (overrideCalendarYear) {
        Box *calendar = findOne<Box>("calendar");
        Port *calendarInitialDateTimePort = calendar->port("initialDateTime");
        QDateTime calendarInitialDateTime = calendarInitialDateTimePort->value<QDateTime>();
        calendarInitialDateTime.setDate(
            QDate(firstDateTime.date().year(),
                  calendarInitialDateTime.date().month(),
                  calendarInitialDateTime.date().day()
            )
        );
        calendarInitialDateTimePort->equals(calendarInitialDateTime);
        calendar->reset();
        calendarDateTime = calendarInitialDateTime;
    }
}

QDateTime Records::alignDateTime(QDateTime dt, int yearOffset) const {
    if (!cycle)
        return dt;
    QDate date = QDate(dt.date().year() + yearOffset, dt.date().month(), dt.date().day());
    return QDateTime(date, dt.time(), Qt::UTC);
}

void Records::advanceLine() {
    advanceTime();
    qSwap(_currentColumnValues, _nextColumnValues);
    readLineItems();

    if (_pastLastLine) {
        _extraYearOffset = 1;
        if (cycle) {
            _file.close();
            openFile();
            readLineItems();
            readLineItems();
            extractValues();
        }
        else {
            extrapolateValues();
        }
    }
    else {
        if (_extraYearOffset == 1) {
            _extraYearOffset = 0;
            ++_yearOffset;
        }
        extractValues();
    }
}

void Records::cleanup() {
    _file.close();
}


} //namespace
