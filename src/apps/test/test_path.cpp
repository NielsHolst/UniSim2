#include <base/path.h>
#include "exception_expectation.h"
#include "test_path.h"

using namespace base;

void TestPath::testFindBox() {
    bool excepted(false);
    Path path;
    try {
        path = Path("Calendar::calendar");
    }
    UNEXPECTED_EXCEPTION;

    auto alternatives = path.alternatives();
    QCOMPARE(alternatives.size(), 1);
    auto alternative = alternatives.at(0);
    QCOMPARE(alternative.nodes().size(), 1);
    auto node = alternative.nodes().at(0);
    QCOMPARE(node.directive(), Path::Directive::Any);
    QCOMPARE(node.className(), QString("Calendar"));
    QCOMPARE(node.objectName(), QString("calendar"));
}
