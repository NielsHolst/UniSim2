#include <QVector>
#include <base/convert.h>
#include "exception_expectation.h"
#include "test_convert_vector.h"

using namespace base;

void TestConvertVector::testIsInstance() {
    bool ok;

    ok = isInstance<QVector<int>, QVector>{};
    QVERIFY(ok);

    ok = isInstance<int, QVector>{};
    QVERIFY(!ok);
}

void TestConvertVector::testBool() {
    bool excepted(false);
    QVector<bool> a, b, c;
    QVector<int>  d;

    b << true << false << true;
    c << false << false << true << true;
    d << 0 << 0 << 7 << 8;

    try {
        a = convert<QVector<bool>>(b);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(a, b);

    try {
        a = convert<QVector<bool>>(d);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(a, c);
}
