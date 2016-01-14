#include <boxes/port.h>
#include <boxes/vectorize.h>
#include "test_vectorization.h"

using namespace boxes;

void TestVectorization::testDifferentTypes() {
    Port a("A"), b("B"), c("C");
    int i(8), j(12);
    double x(100.2);
    a.data(&i);
    b.data(&j);
    c.data(&x);

    QVector<Port*> source;
    source << &a << &b << &c;
    QVector<int> dest, expected;
    const void *destResult = vectorize(Int, source);
    dest = *reinterpret_cast<const QVector<int>*>(destResult);
    expected << 8 << 12 << 100;
    QCOMPARE(dest, expected);
}
