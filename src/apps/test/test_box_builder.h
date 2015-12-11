#ifndef TEST_BOX_BUILDER_H
#define TEST_BOX_BUILDER_H

#include "autotest.h"

namespace boxes {
    class  Box;
}

class TestBoxBuilder : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testBuild1();
private:
    boxes::Box *tree;
};

DECLARE_TEST(TestBoxBuilder)

#endif
