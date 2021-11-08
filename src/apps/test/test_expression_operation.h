#ifndef TEST_EXPRESSION_OPERATION_H
#define TEST_EXPRESSION_OPERATION_H

#include "autotest.h"

class TestExpressionOperation : public QObject
{
    Q_OBJECT
private slots:
    void testAdd();
    void testAddPtr();
};

DECLARE_TEST(TestExpressionOperation)

#endif
