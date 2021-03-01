#ifndef COFFEE_SUMMARY_H
#define COFFEE_SUMMARY_H
#include <base/box.h>

namespace student {

class CoffeeSummary : public base::Box
{
public:
    CoffeeSummary(QString name, QObject *parent);
    void reset();
    void update();
    void cleanup();
private:
    // Inputs
    double harvestTotalOutflow;
    int skipYears;
    QDate date;
    // Outputs
    double average, sd;
    // Data
    QVector<double> harvests;
    double lastHarvest;
    int year;
};

}

#endif
