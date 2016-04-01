#include <base/convert.h>
#include "test_convert.h"

using namespace base;
//
// Vector conversions from QStringList
//

template<class T> QVector<T> convert(QStringList list) {
    QVector<T> result;
    result.reserve(list.size());
    for (QString s : list.toVector())
        result << convert<T>(s);
    return result;
}

template<> QVector<QString> convert(QStringList list);

template<> QVector<QString> convert(QStringList list) {
    return list.toVector();
}

void TestConvert::testFromStringList() {
    QStringList list;
    QVector<int> expected, result;
    list << "7" << "9" << "13";
    expected << 7 << 9 << 13;
    result = convert<int>(list);
    QCOMPARE(result, expected);
}




