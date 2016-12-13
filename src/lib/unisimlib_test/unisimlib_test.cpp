#include <QApplication>
#include <QVector>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/dialog_stub.h>
#include <base/exception.h>
#include <base/object_pool.h>
#include <base/port.h>
#include <base/vector.h>
#include "unisimlib_test.h"

using namespace base;

namespace UniSim {

static int counter = 0;

static int argc = 1;
static char *argv;
static QApplication *app = 0;

static QObject *rootObject = 0;
static QWidget *rootWidget = 0;
static ObjectPool *pool = 0;
static DialogBase *dialog =0;

QVector<int> _sequence;

void _init() {
    if (!rootObject) {
        argv = new char[16];
        *argv = 0;
        app = new QApplication(argc, &argv);
        rootObject = new QObject;
        rootWidget = new QWidget;
        pool = new ObjectPool(rootObject);
        dialog = new DialogStub(rootWidget);
    }
    ++counter;
}

void close() {
    delete rootObject;
    delete rootWidget;
    rootObject = 0;
    rootWidget = 0;
}

Box* buildJump(int start, int length) {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            port("steps").equals(length-1).
            box("Jump").name("jump").
                port("initial").equals(start).
            endbox().
            box("OutputBuffer").
                port("ports").equals("(jump[value])").
            endbox().
        endbox();
    return builder.content();
}

void copyToSequence(const QVector<int> *values) {
    _sequence.resize(values->size());
    int i = 0;
    for (int value : *values)
        _sequence[i++] = value;
}

void runJump(Box *sim) {
    sim->run();

    Port *value = sim->resolveOne<Port>("jump[value]");
    const Vector* vector = value->trackPtr();
    Q_ASSERT(vector);
    const QVector<int> *values = reinterpret_cast<const QVector<int> *>(vector->ptr());
    Q_ASSERT(values);
    copyToSequence(values);
}

Output run1(Input input) {
    _init();

    Box *sim = buildJump(input.start, input.length);
    runJump(sim);
    delete (sim);

    return Output{_sequence.data(), _sequence.size()};
}

void run2(const Input *input, Output *output) {
    _init();

    if (!input)
        ThrowException("run2: input argument is null");
    if (!output)
        ThrowException("run2: output argument is null");

    Box *sim = buildJump(input->start, input->length);
    runJump(sim);
    delete (sim);

    *output =  Output{_sequence.data(), _sequence.size()};
}

}
