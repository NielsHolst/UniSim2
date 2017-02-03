#ifndef SCENARIOS_H
#define SCENARIOS_H

#include <QVector>
#include <base/box.h>
#include <base/data_frame.h>

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
    base::DataFrame _df;
    int _ixRow;
};

}
#endif
