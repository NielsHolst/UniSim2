#ifndef IMMIGRATION_H
#define IMMIGRATION_H
#include <base/box.h>

namespace cmbp {

class Immigration : public base::Box
{
public:
    Immigration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    int day;
    double number, storeMass;
    // Output
    double value;
    // Data
    int _day;
    bool _happened;
};

}

#endif
