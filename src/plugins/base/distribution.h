#ifndef BASE_DISTRIBUTION_H
#define BASE_DISTRIBUTION_H
#include <QObject>
#include <QStringList>

namespace base {

class Distribution : public QObject
{
public:
    Distribution(QString name, QObject *parent = 0);
    void arguments(QStringList args);
    QStringList arguments() const;
    double level(int levelNumber, int noOfLevels);
private:
    QStringList _args;
    virtual int noOfArguments() const = 0;
    virtual void parseArguments(QStringList args) = 0;
    virtual double computeLevel(int levelNumber, int noOfLevels) const = 0;
};

}

#endif
