#ifndef IMMIGRATION_H
#define IMMIGRATION_H
#include <base/box.h>

namespace resist {

class Immigration : public base::Box
{
public:
    Immigration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double q, N, K, proportion;
    // Outputs
    double ss, sr, rr;
};

}

#endif
