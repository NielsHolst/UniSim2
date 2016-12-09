#include <base/convert.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "even.h"

using namespace base;

namespace distribution {

PUBLISH(even)

even::even(QString name, QObject *parent)
    : Distribution(name, parent)
{
}

void even::parseArguments(QStringList args) {
    minValue = convert<double>(args.at(0));
    maxValue = convert<double>(args.at(1));
    if (minValue > maxValue) {
        QString val{"%1 > %2"};
        ThrowException("Minimum value cannot be larger than maximum value")
            .value(val.arg(minValue).arg(maxValue))
            .hint("Swap values")
            .context(this);
    }
}

double even::computeLevel(int levelNumber, int noOfLevels) const {
    return minValue + double(levelNumber)/noOfLevels*(maxValue-minValue);
}

}
