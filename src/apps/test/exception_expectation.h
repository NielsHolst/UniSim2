#ifndef EXCEPTION_EXPECTATION_H
#define EXCEPTION_EXPECTATION_H
#include <exception>
#include <iostream>
#include <qtestcase.h>
#include <base/exception.h>

using namespace base;

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
catch(...) { \
    excepted = true; \
} \
QVERIFY(excepted); \
excepted = false

#define EXPECTED_EXCEPTION_SHOWN \
catch(const Exception &ex) { \
    QString s = "Expected exception:\n" + ex.what(); \
    std::cout << qPrintable(s) << std::endl; \
    excepted = true; \
} \
catch(...) { \
    excepted = true; \
} \
QVERIFY(excepted); \
excepted = false


#endif // EXCEPTION_EXPECTATION_H
