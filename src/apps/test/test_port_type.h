#ifndef TEST_PORT_TYPE_H
#define TEST_PORT_TYPE_H

#include "autotest.h"

class TestPortType : public QObject
{
    Q_OBJECT
private slots:
    void testInt();
    void testDate();
    void testIntVector();
    void testDateVector();
};

DECLARE_TEST(TestPortType)

#endif
