#include <base/publish.h>
#include "forager_recruitment.h"

using namespace base;

namespace apis {

PUBLISH(ForagerRecruitment)

ForagerRecruitment::ForagerRecruitment(QString name, QObject *parent)
    : Box(name, parent)
{
    help("");
    Output(value).help("");
}

void ForagerRecruitment::update() {
}

}
