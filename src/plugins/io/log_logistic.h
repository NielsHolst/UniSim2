#ifndef LOG_LOGISTIC_H
#define LOG_LOGISTIC_H
#include <base/box.h>

namespace io {

class LogLogistic : public base::Box
{
public: 
    LogLogistic(QString name, QObject *parent);
    void initialize();
	void reset();
	void update();

private:
    // Inputs
    double lower, upper, ED50, slope, dose;
    // Outputs
    double value;
    // Methods
    double f(double z);
};

}

#endif
