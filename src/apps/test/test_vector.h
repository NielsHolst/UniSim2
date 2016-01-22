#ifndef TEST_VECTOR_H
#define TEST_VECTOR_H

#include "autotest.h"

class TestVector : public QObject
{
    Q_OBJECT
private slots:
    void testWithBox();
    void testWithSimulation();
    void testWithSimulationIterated();
};

DECLARE_TEST(TestVector)

#endif
