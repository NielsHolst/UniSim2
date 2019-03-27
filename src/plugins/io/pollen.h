#ifndef POLLEN_H
#define POLLEN_H
#include <QDate>
#include <base/box.h>

namespace io {

class Pollen : public base::Box
{
public: 
    Pollen(QString name, QObject *parent);
    void initialize();
	void reset();
	void update();

private:
    // Inputs
    double distance, distanceExp, distanceMin, Npeak, lossRate;
    int duration;
    QDate onsetDate, currentDate;
    // Outputs
    double depositionRate, depositionTotal, density;
    // Data
    double scaleToPeakAtOne;
    // Methods
    void setScaleToPeakAtOne();
    int daysIntoShedding();
    double parabola(int x);
    double distanceEffect();
};

} //namespace
#endif
