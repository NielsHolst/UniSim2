#ifndef TEST_NEW_PORT_H
#define TEST_NEW_PORT_H

#include "autotest.h"

class TestNewPort : public QObject
{
    Q_OBJECT
private slots:
    void testImportNewPort();
    void testImportNewPortVector();
};

DECLARE_TEST(TestNewPort)

#endif
