#ifndef TEST_ANY_YEAR_H
#define TEST_ANY_YEAR_H

#include "autotest.h"

class TestAnyYear : public QObject
{
    Q_OBJECT
private slots:
    void testUnary();
    void testBinaryOneHasAnyYear();
};

DECLARE_TEST(TestAnyYear)

#endif
