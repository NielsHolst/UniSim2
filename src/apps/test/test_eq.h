#ifndef TEST_EQ_H
#define TEST_EQ_H

#include "autotest.h"

class TestEq : public QObject
{
    Q_OBJECT
private slots:
    void testGompertz();
    void testZigmoid();
};

DECLARE_TEST(TestEq)

#endif
