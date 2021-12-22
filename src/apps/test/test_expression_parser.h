#ifndef TEST_EXPRESSION_PARSER_H
#define TEST_EXPRESSION_PARSER_H

#include "autotest.h"

namespace base {
    class  Box;
}

class TestExpressionParser : public QObject
{
    Q_OBJECT
private slots:
    void testAstHandling();
    void testParser();
};

DECLARE_TEST(TestExpressionParser)

#endif
