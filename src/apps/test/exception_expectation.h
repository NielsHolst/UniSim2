#ifndef EXCEPTION_EXPECTATION_H
#define EXCEPTION_EXPECTATION_H
#include <exception>
#include <qtestcase.h>
#include <base/exception.h>

#define UNEXPECTED_EXCEPTION \
catch(const base::Exception &ex) { \
    QString s = "Unexpected exception: " + ex.what(); \
    QFAIL(qPrintable(s)); \
} \
catch (const std::exception& ex) { \
    QString s = "Unexpected exception: " + QString(ex.what()); \
    QFAIL(qPrintable(s)); \
} \
catch (...) { \
    QFAIL("Unexpected exception"); \
} \
QVERIFY(!excepted); \
excepted = false

#define EXPECTED_EXCEPTION \
catch(const Exception &) { \
    excepted = true; \
} \
QVERIFY(excepted); \
excepted = false


#endif // EXCEPTION_EXPECTATION_H
