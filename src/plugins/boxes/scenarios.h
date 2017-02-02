#ifndef SCENARIOS_H
#define SCENARIOS_H

#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QVector>
#include <base/box.h>

namespace base {
class DataFrame;
}

namespace boxes {

class Scenarios : public base::Box
{
public:
    Scenarios(QString name, QObject *parent);
    void amend();
    void reset();

private:
    // inputs
    QString fileName;
    // outputs
    QVector<QString> values;
    bool atEnd;
    // methods
    void readDataFrame();
    void createColumnOutputs();
    void copyValues();
    // data
    base::DataFrame *_df;
    int _ixRow;
};

}
#endif
