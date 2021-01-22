#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <base/box.h>

namespace saccharina {

class Calibration : public base::Box
{
public:
    Calibration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QString obsFileName;
    QDate date;
    double area, yield, pctN, pctC;
    // Data
    QDate nextDate;
    QVector<QDate> dates;
    struct Output {
        // Buffers
        QVector<double> obs, pred;
        // Outputs
        double sumSq;
        // Methods
        void clear();
        void update(double predicted);
    };
    struct {
        Output area, yield, pctN, pctC;
    } output;
    int row;
};

}

#endif
