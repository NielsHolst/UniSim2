#ifndef BASE_DISTRIBUTION_H
#define BASE_DISTRIBUTION_H
#include <QObject>
#include <QStringList>

namespace base {

class Distribution : public QObject
{
public:
    Distribution(QString name, QObject *parent = 0);
    void arguments(QStringList args);
    QStringList arguments() const;
private:
    QStringList _args;
};

}

#endif
