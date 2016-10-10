#ifndef SCENARIOS_H
#define SCENARIOS_H

#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QVector>
#include <base/box.h>
#include <base/environment.h>

namespace boxes {

class Scenarios : public base::Box
{
public:
    Scenarios(QString name, QObject *parent);
    void amend();
    void initialize();
    void reset();
    void debrief();

private:
    // inputs
    QString fileName, separator;
    // outputs
    QVector<QString> values;
    bool atEnd;
    // methods
    void openFile();
    void readColumnNames();
    void createColumnOutputs();
    void readLineItems();
    void updateValues();
    void checkSeparator();
    // data
	QFile file;
    QStringList lineItems, columnNames;
    bool pastLastLine;
};

}
#endif
