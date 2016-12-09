#ifndef EVEN_H
#define EVEN_H
#include <base/distribution.h>

namespace distribution {

class even : public base::Distribution
{
public:
    even(QString name, QObject *parent);
private:
    double minValue, maxValue;
    int noOfArguments() const { return 2; }
    void parseArguments(QStringList args);
    double computeLevel(int levelNumber, int noOfLevels) const;
};

}

#endif
