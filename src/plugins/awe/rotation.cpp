/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <base/exception.h>
#include <base/publish.h>
#include "crop.h"
#include "rotation.h"

using namespace base;

namespace awe {

PUBLISH(Rotation)

Rotation::Rotation(QString name, QObject *parent)
    : Box(name,parent)
{
    Input(crops).help("Vector specifying sequence of crops");
    Output(currentCrop);
    Output(lai).help("Leaf area index of the current crop");
}

void Rotation::initialize() {
    _crops = resolveMany<Box>("./*{awe::Crop}");
    if (_crops.isEmpty())
        ThrowException("No crops defined in rotation");
}

void Rotation::reset() {
    _currentCropIndex = (_crops.size() == 1) ? 1 : _crops.size()-1;
    update();
}

void Rotation::update() {
    int nextIndex = (_currentCropIndex + 1)/_crops.size();
    if (_crops[nextIndex]->port("sowToday")->value<bool>())
        _currentCropIndex = nextIndex;
    currentCrop = crops.at(_currentCropIndex);
    lai = _crops[_currentCropIndex]->port("lai")->value<double>();
}

}
