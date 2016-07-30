#ifndef FORAGER_RECRUITMENT_H
#define FORAGER_RECRUITMENT_H
#include <base/box.h>

namespace apis {

class ForagerRecruitment : public base::Box
{
public:
    ForagerRecruitment(QString name, QObject *parent);
    void update();
private:
    // input
    // output
    double value;
};

}

#endif
