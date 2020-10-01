#include <QSet>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/track.h>
#include "exception_expectation.h"
#include "test_random.h"

using namespace base;

void TestRandom::testDrawAt() {
    bool excepted(false);
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "random/draw_at.box", nullptr);
    QCOMPARE(errors, dialog().errorCount());

    Box *sim;
    Port *a, *b, *c, *d, *e, *f, *g;
    double a1, b1, c1, d1, e1, f1, g1,
           a2, b2, c2, d2, e2, f2, g2,
           a3, b3, c3, d3, e3, f3, g3;
    try {
        sim = environment().root();
        Box *client = sim->findOne<Box>("client");
        a = client->port("a");
        b = client->port("b");
        c = client->port("c");
        d = client->port("d");
        e = client->port("e");
        f = client->port("f");
        g = client->port("g");
    }
    UNEXPECTED

    // They must all be different, except (d,e) and (f,g) which draw from common boxes
    try {
        Track::effectuateOrders();
        environment().computationStep(ComputationStep::Initialize);
        sim->initializeFamily();
        a1 = a->value<double>();
        b1 = b->value<double>();
        c1 = c->value<double>();
        d1 = d->value<double>();
        e1 = e->value<double>();
        f1 = f->value<double>();
        g1 = g->value<double>();
    }
    UNEXPECTED
    QSet<double> all;
    all << a1 << b1 << c1 << d1 << f1;
    QCOMPARE(all.size(), 5);
    QCOMPARE(d1, e1);
    QCOMPARE(f1, g1);

    // The atReset and atUserReset must have changed
    try {
        environment().computationStep(ComputationStep::Reset);
        sim->resetFamily();
        Track::resetAll();
        a2 = a->value<double>();
        b2 = b->value<double>();
        c2 = c->value<double>();
        d2 = d->value<double>();
        e2 = e->value<double>();
        f2 = f->value<double>();
        g2 = g->value<double>();
    }
    UNEXPECTED
    QCOMPARE(a1, a2);
    QVERIFY(b1!=b2);
    QCOMPARE(c1, c2);
    QVERIFY(d1!=d2);
    QVERIFY(e1!=e2);
    QVERIFY(d2!=e2);
    QCOMPARE(f1, f2);
    QCOMPARE(g1, g2);

    QFAIL("No Track::updateAll() method");
    // The atUpdate and atUserUpdate must have changed
//    try {
//        environment().computationStep(ComputationStep::Update);
//        sim->updateFamily();
//        Track::updateAll();
//        a3 = a->value<double>();
//        b3 = b->value<double>();
//        c3 = c->value<double>();
//        d3 = d->value<double>();
//        e3 = e->value<double>();
//        f3 = f->value<double>();
//        g3 = g->value<double>();
//    }
//    UNEXPECTED
//    QCOMPARE(a2, a3);
//    QCOMPARE(b2, b3);
//    QVERIFY(c2!=c3);
//    QCOMPARE(e2, d3); //latest value in atUserReset
//    QCOMPARE(e2, e3);
//    QVERIFY(f2!=f3);
//    QVERIFY(g2!=g3);
//    QVERIFY(f3!=g3);

//    // Must run OK
//    try {
//        sim->run();
//    }
//    UNEXPECTED
}
