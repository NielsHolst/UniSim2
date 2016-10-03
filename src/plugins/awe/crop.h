#ifndef CROP_H
#define CROP_H

#include <QMap>
#include <base/box.h>

namespace awe {

class Crop : public base::Box
{
    Q_OBJECT
public:
    Crop(QString name, QObject *parent=0);

    void initialize();
    void reset();
    void update();

private:
    // inputs
    QString currentCrop;
    QDate sowingDate, harvestDate, date;
    double weedExchangeRate, maxYield,
           maxYieldLossPct, slopeYieldLossPct, ddSum;
    QVector<double> laiX, laiY;

    // outputs
    double lai;
    bool sowToday, harvestToday;

    // data
    QMap<double, double> laiXY;

    // methods
    double interpolateLai() const;
};

}
#endif
