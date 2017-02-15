#include <QSet>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include "test_random.h"

using namespace base;

void TestRandom::testDrawAt() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "random/draw_at.box", 0);
    QCOMPARE(errors, dialog().errorCount());

    environment().computationStep(ComputationStep::Construct);
    Box *sim = environment().root(),
        *client = sim->findOne<Box>("client");
    Port *a = client->port("a"),
         *b = client->port("b"),
         *c = client->port("c"),
         *d = client->port("d"),
         *e = client->port("e"),
         *f = client->port("f"),
         *g = client->port("g");

    // They must all be different, except (d,e) and (f,g) which draw from common boxs
    environment().computationStep(ComputationStep::Initialize);
    sim->initializeFamily();
    double a1 = a->value<double>(),
           b1 = b->value<double>(),
           c1 = c->value<double>(),
           d1 = d->value<double>(),
           e1 = e->value<double>(),
           f1 = f->value<double>(),
           g1 = g->value<double>();
    QSet<double> all;
    all << a1 << b1 << c1 << d1 << f1;
    QCOMPARE(all.size(), 5);
    QCOMPARE(d1, e1);
    QCOMPARE(f1, g1);

    // The atReset and atUserReset must have changed
    environment().computationStep(ComputationStep::Reset);
    sim->resetFamily();
    double a2 = a->value<double>(),
           b2 = b->value<double>(),
           c2 = c->value<double>(),
           d2 = d->value<double>(),
           e2 = e->value<double>(),
           f2 = f->value<double>(),
           g2 = g->value<double>();
    QCOMPARE(a1, a2);
    QVERIFY(b1!=b2);
    QCOMPARE(c1, c2);
    QVERIFY(d1!=d2);
    QVERIFY(e1!=e2);
    QVERIFY(d2!=e2);
    QCOMPARE(f1, f2);
    QCOMPARE(g1, g2);

    // The atUpdate and atUserUpdate must have changed
    environment().computationStep(ComputationStep::Update);
    sim->updateFamily();
    double a3 = a->value<double>(),
           b3 = b->value<double>(),
           c3 = c->value<double>(),
           d3 = d->value<double>(),
           e3 = e->value<double>(),
           f3 = f->value<double>(),
           g3 = g->value<double>();
    QCOMPARE(a2, a3);
    QCOMPARE(b2, b3);
    QVERIFY(c2!=c3);
    QCOMPARE(e2, d3); //latest value in atUserReset
    QCOMPARE(e2, e3);
    QVERIFY(f2!=f3);
    QVERIFY(g2!=g3);
    QVERIFY(f3!=g3);

    // Must run OK
    try {
        sim->run();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}
