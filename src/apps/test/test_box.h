#ifndef TEST_BOX_H
#define TEST_BOX_H

#include "autotest.h"

class TestBox : public QObject
{
    Q_OBJECT
private slots:
    void testCreate();
    void testVectorInput();
    void testSiblingImport();
    void testParentImport();
};

DECLARE_TEST(TestBox)

#endif
