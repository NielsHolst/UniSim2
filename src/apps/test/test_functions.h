#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
#include "autotest.h"

class TestFunctions : public QObject
{
    Q_OBJECT
private slots:
    void test_c();
};

DECLARE_TEST(TestFunctions)

#endif
