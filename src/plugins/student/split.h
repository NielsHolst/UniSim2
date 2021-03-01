#ifndef SPLIT_H
#define SPLIT_H
#include <base/box.h>

namespace student {

class Split : public base::Box
{
public: 
    Split(QString name, QObject *parent);
    void amend();
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double whole;
    QVector<double> proportions;
    // Outputs
    QVector<double> pieces;
    // Data
    int n;
};

} //namespace
#endif
