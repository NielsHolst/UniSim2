#ifndef BIG_BANG_H
#define BIG_BANG_H
#include <QVector>
#include <base/box.h>

namespace student {

class BigBang : public base::Box
{
public:
    BigBang(QString name, QObject *parent);
    void amend();
    void reset();
private:
    // Input
    QString connectivityFile;
    // Data
    QVector<Box*> plants, pollinators;
    // Methods
    void setPlantParameters(Box *plant);
    void setPollinatorParameters(Box *pollinator);
    double draw(QString boxPath);
};

}
#endif
