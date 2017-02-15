#ifndef MAKER_H
#define MAKER_H

#include <base/box.h>
#include <base/data_frame.h>

namespace base {
class DataFrame;
}

namespace boxes {

class Maker : public base::Box
{
public:
    Maker(QString name, QObject *parent);
    void amend();

private:
    // inputs
    int replicates;
    QVector<QString> names;
    QString fileName;
    // data
    base::DataFrame _df;
    bool _cloneFromFile;
    QVector<QString> _columnPaths;
    // methods
    void readDataFrame();
    void copyPortValuesFromColumns(int row);
};

}
#endif
