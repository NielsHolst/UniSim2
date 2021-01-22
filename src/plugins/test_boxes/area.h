#ifndef AREA_H
#define AREA_H
#include <base/box.h>

namespace test_boxes {

class Area : public base::Box
{
public:
    Area(QString name, QObject *parent);
};

}

#endif
