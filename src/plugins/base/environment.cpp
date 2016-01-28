#include <QDir>
#include "environment.h"


namespace base {

Environment::Environment()
{
    state.dir = QDir::home();
}

}
