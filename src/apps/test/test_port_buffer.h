#ifndef TEST_PORT_BUFFER_H
#define TEST_PORT_BUFFER_H

#include "autotest.h"

class TestPortBuffer : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void testCreateBool();

    void testImportNoBuffer();
    void testImportFromBuffer();
    void testImportToBuffer();
    void testImportBufferToBuffer();

    void testImportVectorNoBuffer();
    void testImportVectorFromBuffer();
    void testImportVectorToBuffer();
    void testImportVectorBufferToBuffer();
};

DECLARE_TEST(TestPortBuffer)

#endif
