#ifndef BASE_DOCUMENTATION_H
#define BASE_DOCUMENTATION_H
#include "box.h"

namespace base {

class Documentation : public base::Box
{
public:
    Documentation(QString name, QObject *parent);
protected:
    // Inputs
    QString
        title;
    QVector<QString> authorNames,
                     authorInstitutes,
                     authorEmails;
};

} //namespace

#endif
