#ifndef EXCEPTION_EXPECTATION_H
#define EXCEPTION_EXPECTATION_H

#include <qtestcase.h>

#define UNEXPECTED \
catch(Exception &ex) { \
    QString s = "Unexpected exception: " + ex.what(); \
    QFAIL(qPrintable(s)); \
}

#define EXPECTED \
catch(Exception &ex) { \
    excepted = true; \
} \
QVERIFY(excepted);


#endif // EXCEPTION_EXPECTATION_H
