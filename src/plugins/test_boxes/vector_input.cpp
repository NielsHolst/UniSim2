#include <base/publish.h>
#include "vector_input.h"

using namespace base;

namespace test_boxes {

PUBLISH(VectorInput)

VectorInput::VectorInput(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(numbers1);
    Input(numbers2);
}

}
