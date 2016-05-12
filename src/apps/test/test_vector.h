#ifndef TEST_VECTOR_H
#define TEST_VECTOR_H

#include "autotest.h"

namespace base {
    class DialogStub;
}

class TestVector : public QObject
{
    Q_OBJECT
private slots:
    void testWithSimulation();
    void testWithSimulationIterated();
private:
    base::DialogStub *dialogStub;
};

DECLARE_TEST(TestVector)

#endif
