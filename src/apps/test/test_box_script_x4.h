#ifndef TEST_BOX_SCRIPT_X3_H
#define TEST_BOX_SCRIPT_X3_H

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
