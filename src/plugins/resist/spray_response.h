#ifndef SPRAY_RESPONSE_H
#define SPRAY_RESPONSE_H
#include <base/box.h>

namespace resist {

class SprayResponse : public base::Box
{
public:
    SprayResponse(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double a,b,c,coverage,S;
    // Output
    double ss, sr, rr;
};

}

#endif
