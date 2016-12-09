#ifndef NORMAL_H
#define NORMAL_H
#include <base/distribution.h>

namespace distribution {

class normal : public base::Distribution
{
public:
    normal(QString name, QObject *parent);
private:
    double mean, sd;
    int noOfArguments() const { return 2; }
    void parseArguments(QStringList args);
    double computeLevel(int levelNumber, int noOfLevels) const;
};

}

#endif
