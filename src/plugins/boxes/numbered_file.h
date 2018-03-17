#ifndef NUMBERED_FILE_H
#define NUMBERED_FILE_H
#include <base/box.h>

namespace boxes {

class NumberedFile : public base::Box
{
public:
    NumberedFile(QString name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Input
    QString fileName;
    int number, numberWidth;
    // Output
    QString value;
};

}

#endif
