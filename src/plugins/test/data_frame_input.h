#ifndef DATA_FRAME_INPUT_H
#define DATA_FRAME_INPUT_H
#include <base/box.h>

namespace test {

class DataFrameInput : public base::Box
{
public:
    DataFrameInput(QString name, QObject *parent);
    void amend();
private:
    QString dfFileName;
    int defaultValue;
};

}

#endif
