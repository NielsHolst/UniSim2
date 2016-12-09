#include "exception.h"
#include "distribution.h"

namespace base {

Distribution::Distribution(QString name, QObject *parent)
    : QObject(parent)
{
    setObjectName(name);
}

void Distribution::arguments(QStringList args) {
    _args = args;
    int n = _args.size();
    if (n != noOfArguments()) {
        QString msg{"Even distribution needs %1 arguments got %2"};
        ThrowException(msg.arg(noOfArguments()).arg(n))
            .value(_args.join(" "))
            .context(this);
    }
    parseArguments(args);
}

QStringList Distribution::arguments() const {
    return _args;
}

double Distribution::level(int levelNumber, int noOfLevels) {
    if (levelNumber<0 || levelNumber>noOfLevels)
        ThrowException("Level must be inside [0;noOfLevels]").value(levelNumber).context(this);
    return computeLevel(levelNumber, noOfLevels);
}


}
