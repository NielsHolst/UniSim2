#include <QSet>
#include <boxes/box.h>
#include <boxes/exception.h>
#include <boxes/general.h>
#include <boxes/path.h>
#include "test_box_cases.h"
#include "test_path.h"

using namespace boxes;
using namespace std;

void TestPath::initTestCase() {
    _root = TestBoxCases::case1();
}

void TestPath::cleanupTestCase() {
    delete _root;
}

void TestPath::setContext(QString path) {
    if (path.isEmpty()) {
        _context = 0;
        return;
    }
    Path p{path};
    QVector<QObject*> objects;
    try {
        objects = p.resolve();
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    int n = objects.length();
    QString msg{"Wrong number of box matches in '%1'. Expected 1 but found %2"};
    msg = msg.arg(path).arg(n);
    QVERIFY2(n==1, msg.toLocal8Bit());
    _context = objects.at(0);
}

namespace {
    QString names(QSet<QObject*> set) {
        QStringList s;
        for (QObject *item : set) {
            s << fullName(item);
        }
        return s.join(" ");
    }
}

void TestPath::compareVectors(QVector<QObject*> v1, QVector<QObject*> v2, int size) {
    QSet<QObject*> set1 = QSet<QObject*>::fromList( QList<QObject*>::fromVector(v1) ),
                   set2 = QSet<QObject*>::fromList( QList<QObject*>::fromVector(v2) );
    QString msgNames = "Object sets differ:\n(%1)\n(%2)",
            msgSize = "Object set(s) of wrong size. Excepted %1 got %2 and %3.\nExpected: '%4'";
    QString s1 = msgNames.arg(names(set1)).arg(names(set2));
    QString s2 = msgSize.arg(size).arg(set1.size()).arg(set2.size());

    QVERIFY2(set1==set2, msgNames.arg(names(set1)).arg(names(set2)).toLocal8Bit());

    QVERIFY2(set1.size()==size, msgSize.arg(size).arg(set1.size()).arg(set2.size()).arg(names(set2)).toLocal8Bit());
}

void TestPath::testValidateName() {
    Path path("/A/A2");
    try {
        path.validateName("ABC");
        path.validateName("*");
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }

    bool excepted = false;
    try {
        path.validateName(".");
        path.validateName("A*");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPath::testValidateStep() {
    Path path("/A/A2");
    try {
        path.validateStep("ABC");
        path.validateStep("ABC:");
        path.validateStep("*");
        path.validateStep(".");
        path.validateStep("..");
        path.validateStep("...");
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }

    bool excepted = false;
    try {
        path.validateName(".A");
        path.validateName("A:B");
        path.validateStep("....");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestPath::testNormalise() {
    QString s = Path("plant/fruit/area[v]").normalise();
    QCOMPARE(s,
         QString("selfordescendants:plant{Box}/children:fruit{Box}/children:area{Box}/children:v{Port}"));

    QCOMPARE(Path("./plant/fruit/area[v]").normalise(),
        QString("self:*{Box}/children:plant{Box}/children:fruit{Box}/children:area{Box}/children:v{Port}"));

    QCOMPARE(Path("../plant/fruit/area[v]").normalise(),
        QString("parent:*{Box}/children:plant{Box}/children:fruit{Box}/children:area{Box}/children:v{Port}"));

    QCOMPARE(Path("/plant/fruit/area[v]").normalise(),
        QString("self:plant{Box}/children:fruit{Box}/children:area{Box}/children:v{Port}"));

    QCOMPARE(Path(".[v]").normalise(),
        QString("self:*{Box}/children:v{Port}"));

    QCOMPARE(Path("/").normalise(),
        QString("self:*{Box}"));

    QCOMPARE(Path("/plant").normalise(),
        QString("self:plant{Box}"));

    QCOMPARE(Path("/plant[v]").normalise(),
        QString("self:plant{Box}/children:v{Port}"));

    QCOMPARE(Path("../plant[v]").normalise(),
        QString("parent:*{Box}/children:plant{Box}/children:v{Port}"));

    QCOMPARE(Path(".../plant[v]").normalise(),
        QString("nearest:*{Box}/children:plant{Box}/children:v{Port}"));
}

void TestPath::testNormaliseInvalid() {
}

void TestPath::testSetContext() {
    setContext("A2");
    QCOMPARE(_context->objectName(), QString("A2"));
}

void TestPath::testSelf() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("self:*", _context).resolve(),
    absolute = Path("/A/A2").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("self:*{Box}", _context).resolve(),
    absolute = Path("/A/A2").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path(".[v2]", _context).resolve(),
    absolute = Path("/A/A2[v2]").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("./b[v1]", _context).resolve(),
    absolute = Path("/A/A2/b[v1]").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("./*[*]", _context).resolve(),
    absolute = Path("/A/A2/*[*]").resolve();
    compareVectors(relative, absolute, 2);

    QCOMPARE(Path("self:X", _context).resolve().size(), 0);
}

void TestPath::testChildren() {
    setContext("A1");
    QVector<QObject*> relative, absolute;

    relative = Path("children:a[v1]", _context).resolve(),
    absolute = Path("/A/A1/a[v1]").resolve();
    compareVectors(relative, absolute, 2);

    QVERIFY(Path("children:x[v1]", _context).resolve().isEmpty());
}

void TestPath::testParent() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("../*", _context).resolve(),
    absolute = Path("/A/*").resolve();
    compareVectors(relative, absolute, 3);

    relative = Path("..[v3]", _context).resolve(),
    absolute = Path("/A[v3]").resolve();
    compareVectors(relative, absolute, 1);

    QVERIFY(Path("parent:X", _context).resolve().isEmpty());
}

void TestPath::testNearest() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path(".../c[v2]", _context).resolve(),
    absolute = Path("/A/A2/c[v2]").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("...[v2]", _context).resolve(),
    absolute = Path("/A/A2[v2]").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("...[v3]", _context).resolve(),
    absolute = Path("/A[v3]").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path(".../*{Port}", _context).resolve(),
    absolute = Path("/A/A2[v2]").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path(".../*{Box}", _context).resolve(),
    absolute = Path("/A/A2/a").resolve();
    compareVectors(relative, absolute, 1);
}

void TestPath::testDescendants() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("descendants:c[v2]", _context).resolve(),
    absolute = Path("/A/A2/c[v2]").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("descendants:*[*]", _context).resolve(),
    absolute = Path("/A/A2/*[*]").resolve();
    compareVectors(relative, absolute, 2);

    QVERIFY(Path("descendants:x[*]", _context).resolve().isEmpty());
}

void TestPath::testAncestors() {
    setContext("A2/c");
    QVector<QObject*> relative, absolute;

    relative = Path("ancestors:A", _context).resolve(),
    absolute = Path("/A").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("ancestors:A2", _context).resolve(),
    absolute = Path("/A/A2").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("ancestors:*", _context).resolve(),
    absolute = Path("/A/A2").resolve() << Path("/A").resolve();
    compareVectors(relative, absolute, 2);

    relative = Path("ancestors:A[v1]", _context).resolve(),
    absolute = Path("/A[v1]").resolve();
    compareVectors(relative, absolute, 1);

    QVERIFY(Path("ancestors:A3", _context).resolve().isEmpty());
}

void TestPath::testAllSiblings() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("allSiblings:*", _context).resolve(),
    absolute = Path("/A/*").resolve();
    compareVectors(relative, absolute, 3);

    setContext("/A");
    QVERIFY(Path("allSiblings:*", _context).resolve().isEmpty());
}

void TestPath::testOtherSiblings() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("otherSiblings:*", _context).resolve(),
    absolute = Path("/A/A1").resolve() << Path("/A/A3").resolve();
    compareVectors(relative, absolute, 2);

    setContext("/A");
    QVERIFY(Path("otherSiblings:*", _context).resolve().isEmpty());
}

void TestPath::testPreceedingSibling() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("preceedingSibling:*", _context).resolve(),
    absolute = Path("/A/A1").resolve();
    compareVectors(relative, absolute, 1);

    setContext("/A");
    QVERIFY(Path("preceedingSibling:*", _context).resolve().isEmpty());
}

void TestPath::testFollowingSibling() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("followingSibling:*", _context).resolve(),
    absolute = Path("/A/A3").resolve();
    compareVectors(relative, absolute, 1);

    setContext("/A");
    QVERIFY(Path("followingSibling:*", _context).resolve().isEmpty());
}

void TestPath::testPathList() {
    setContext("A2");
    QStringList relPaths, absPaths;
    relPaths << "A3[v3]" << "A[*]";
    absPaths << "/A/A3[v3]" << "/A[*]";

    QVector<QObject*> relative, absolute;
    relative = Path(relPaths, _context).resolve(),
    absolute = Path(absPaths).resolve();
    compareVectors(relative, absolute, 3);
}

void TestPath::testGlobal() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("A3[v3]", _context).resolve(),
    absolute = Path("/A/A3[v3]").resolve();
    compareVectors(relative, absolute, 1);
}

void TestPath::testRoot() {
    setContext("A2");
    QVector<QObject*> relative, absolute;

    relative = Path("A", _context).resolve(),
    absolute = Path("/").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("A", _context).resolve(),
    absolute = Path("/A").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("A[v1]", _context).resolve(),
    absolute = Path("/A[v1]").resolve();
    compareVectors(relative, absolute, 1);

    relative = Path("A[*]", _context).resolve(),
    absolute = Path("/A[*]").resolve();
    compareVectors(relative, absolute, 2);
}

void TestPath::testNumberOfMatches() {
    QString path("/A/A2/*[*]");

    bool excepted{false};
    try {
        Path(path).resolve(1);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        Path(path).resolve(2);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(!excepted);
}

void TestPath::testEmpty() {
    QVector<QObject*> none;
    try {
        Path("").resolve(0);
        none = Path("").resolve();
    }
    catch (Exception &ex) {
        QFAIL(ex.what());
    }
    QCOMPARE(none.size(), 0);
}

void TestPath::testResolveInvalid() {
    bool excepted{false};
    try {
        Path(".[v2]").resolve();
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}
