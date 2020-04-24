/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RADIATION_LAYERS_H
#define RADIATION_LAYERS_H

#include <base/box.h>

namespace vg {

class RadiationLayer;
class AirSpaceBase;

class RadiationLayers : public base::Box
{
public:
    RadiationLayers(QString name, QObject *parent);
    void amend();
    void initialize();
    void reset();
    void update();
    // Methods mimicking R functions
    typedef QVector<double> Vec;
    static void distributeRadiation(
            Vec a, Vec r, Vec t,
            Vec a_, Vec r_, Vec t_,
            Vec &I, Vec &I_, Vec &A, Vec &A_
            );
    static void distributeRadiationDown(
            Vec a, Vec r, Vec t,
            Vec a_, Vec r_, Vec t_,
            Vec &I, Vec &I_, Vec &A, Vec &A_
            );
    static void distributeRadiationUp(
            Vec a, Vec r, Vec t,
            Vec a_, Vec r_, Vec t_,
            Vec &I, Vec &I_, Vec &A, Vec &A_
            );
private:
    // Inputs
    double groundArea, cropCoverage,
        outdoorsTemperature, indoorsTemperature, soilTemperature;
    bool withCrop;
    // Outputs
    double area, newIndoorsTemperature;
    // Data
    typedef QVector<RadiationLayer*> Stack;
    typedef QVector<AirSpaceBase*> AirSpaces;
    Stack stack;
    AirSpaces airSpaces;
    // Methods
    void distributeParRadiation(Stack &stack);
    void distributeSwRadiation(Stack &stack);
    void distributeLwRadiation(Stack &stack);
    void distributeHeatByConvection(Stack &stack);
    void distributeHeatByConduction(Stack &stack);
//    void heatTransferOuterSurface(double fromTemperature, double toTemperature) const;
//    void heatTransferInnerSurface(double fromTemperature, double toTemperature) const;
};

} //namespace



#endif
