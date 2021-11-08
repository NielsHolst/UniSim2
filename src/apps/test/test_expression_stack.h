#ifndef TEST_EXPRESSION_STACK_H
#define TEST_EXPRESSION_STACK_H

#include "autotest.h"

class TestExpressionStack : public QObject
{
    Q_OBJECT
private slots:
    void testPush();
    void testReduce();
    void testToPostfix();
};

DECLARE_TEST(TestExpressionStack)

#endif
