#ifndef BIG_BANG_H
#define BIG_BANG_H
#include <QVector>
#include <base/box.h>

namespace pollnet {

class BigBang : public base::Box
{
public:
    BigBang(QString name, QObject *parent);
    void amend();
private:
    // Input
    QString connectivityFile;
    // Data
    QVector<Box*> plants, pollinators;
};

}
#endif
