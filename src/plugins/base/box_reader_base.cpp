/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "box.h"
#include "box_reader_base.h"
#include "exception.h"

namespace base {

BoxReaderBase::BoxReaderBase(BoxBuilder *builder)
    : _builder(builder)
{
}

QString BoxReaderBase::currentInfo() const {
    QString info{"\n  Last box: '%1'\n  Last port: '%2'"};
    QString lastBox = _builder->currentBox() ? _builder->currentBox()->objectName() : QString();
    QString lastPort = _builder->currentPort() ? _builder->currentPort()->objectName() : QString();
    return info.arg(lastBox).arg(lastPort);
}

} // namespace
