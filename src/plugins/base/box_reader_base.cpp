#include "box.h"
#include "box_reader_base.h"
#include "exception.h"

namespace base {

BoxReaderBase::BoxReaderBase()
{
}

QString BoxReaderBase::currentInfo() const {
    QString info{"\n  Last box: '%1'\n  Last port: '%2'"};
    QString lastBox = _builder.currentBox() ? _builder.currentBox()->objectName() : QString();
    QString lastPort = _builder.currentPort() ? _builder.currentPort()->objectName() : QString();
    return info.arg(lastBox).arg(lastPort);
}

QString BoxReaderBase::blankAsNa(QString s) {
    return (s.isEmpty()) ? QString("NA") : s;
}

} // namespace
