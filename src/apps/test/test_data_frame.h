#ifndef TEST_DATA_FRAME_H
#define TEST_DATA_FRAME_H

#include "autotest.h"

class TestDataFrame : public QObject
{
    Q_OBJECT
private slots:
    void testDimensions();
    void testContent();
};

DECLARE_TEST(TestDataFrame)

#endif
