#ifndef ROTATION_H
#define ROTATION_H

#include <QVector>
#include <base/box.h>

namespace awe {

class Crop;

class Rotation : public base::Box
{
public:
    Rotation(QString name, QObject *parent=0);

    void initialize();
    void reset();
    void update();

private:
    // inputs
    QVector<QString> crops;
    // output
    QString currentCrop;
    double lai;
    // state
    QVector<base::Box*> _crops;
    int _currentCropIndex;
};

}
#endif
