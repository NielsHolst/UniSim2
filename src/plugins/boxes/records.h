#ifndef RECORDS_H
#define RECORDS_H

#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QVector>
#include <base/box.h>
#include <base/environment.h>

namespace boxes {

class Records : public base::Box
{
public:
    Records(QString name, QObject *parent);
    ~Records();
    // standard methods
    void amend();
    void reset();
    void update();
    void cleanup();

private:
    // inputs
    QString fileName, dateColumnName, timeColumnName;
    QDateTime calendarDateTime;

    // outputs
    QDateTime currentDateTime, nextDateTime, firstDateTime, lastDateTime;
    QDate currentDate, nextDate;
    QTime currentTime, nextTime;
    QVector<double> values;

    // methods
    QString fileNamePath();
    void openFile();
    void readLineItems();
    void readColumnNames();
    void createColumnOutputs();
    void readFromFirstToLastLine();
    void readToFirstLine();
    void advanceFirstLine();
    void advanceLine();
    void extractValues();
    void advanceTime();

    // data
	QFile file;
    QStringList lineItems, columnNames;
    bool pastLastLine;
    int dateColumn, timeColumn;
    QVector<double> *currentColumnValues, *nextColumnValues;
};

}
#endif
