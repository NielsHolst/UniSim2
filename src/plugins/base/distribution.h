#ifndef BASE_DISTRIBUTION_H
#define BASE_DISTRIBUTION_H
#include <QObject>
#include <QPair>
#include <QStringList>

namespace base {

class Port;

class Distribution : public QObject
{
public:
    Distribution(QString name, QObject *parent = 0);
    virtual ~Distribution() {}
    void arguments(QStringList args);
    virtual void mean(double value);
    virtual void sd(double value);
    virtual void min(double value);
    virtual void max(double value);
    virtual void lowerQuantile(double value);
    virtual void upperQuantile(double value);
    virtual double draw() = 0;
    void port(Port *port_);
    Port* port();
protected:
    void parseNext(double *value);
    bool _isFirstDraw;
private:
    virtual void parseArguments() = 0;
    void unavailable(QString method, double value);
    QStringList _args;
    int _ixNext;
    Port *_port;
};

}

#endif
