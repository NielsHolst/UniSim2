#ifndef STUDENT_DOCUMENTATION_H
#define STUDENT_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace student {

class studentDocumentation : public base::Documentation
{
public:
    studentDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
