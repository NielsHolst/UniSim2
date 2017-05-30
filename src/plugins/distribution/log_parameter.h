#ifndef LOG_PARAMETER_H
#define LOG_PARAMETER_H

class Object;

namespace distribution {

struct LogParameter
{
    // Methods
    LogParameter(QObject *parent);
    void transform();
    // Data
    double value, tr;
private:
    QObject *_parent;
};

}

#endif
