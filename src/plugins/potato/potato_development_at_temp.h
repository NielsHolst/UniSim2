//definining headers
#ifndef POTATO_DEVELOPMENT_AT_TEMP_H
#define POTATO_DEVELOPMENT_AT_TEMP_H
#include <base/box.h>

namespace potato {

class PotatoDevelopmentAtTemp : public base::Box
{
public:
    PotatoDevelopmentAtTemp(QString name, QObject *parent);
    void reset();
    void update();
private:
    //inputs
    double temp, Topt, Tfloor, Tceiling;
    // outputs
    double step;
};

}

#endif
