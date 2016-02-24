#ifndef PAGE_R_H
#define PAGE_R_H
#include <base/box.h>

namespace boxes {

class PageR : public base::Box
{
public:
    PageR(QString name, QObject *parent);
private:
    // Inputs
    QString xAxis;
    bool overlay;
    int width, height, ncol;
};

}

#endif
