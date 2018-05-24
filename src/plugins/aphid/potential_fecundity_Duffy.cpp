#include <base/publish.h>
#include "potential_fecundity_Duffy.h"

using namespace base;

namespace aphid {

PUBLISH(Potential_fecundity_Duffy)

Potential_fecundity_Duffy::Potential_fecundity_Duffy(QString name, QObject *parent)
    : Box(name, parent)
{
    Input (Temperature);
    Input (Wheat_Pheno);
    Output(alate_potential);
    Output(apterous_potential);
}

void Potential_fecundity_Duffy::reset() {
    alate_potential=
    apterous_potential=0;
}

void Potential_fecundity_Duffy::update() {
    // TEMPERATURE EFFECT ON FECUNDITY FOR ALATES ONLY

    if(Temperature<3 || Temperature>30)
    {alate_potential=
    apterous_potential=0;}

    if(Temperature>3 && Temperature<=20)
    {alate_potential=-0.3653 + 0.1218*Temperature;
    apterous_potential=-0.3766 + 0.1772*Temperature;}

    if(Temperature>20 && Temperature<=30)
    {alate_potential=6.21 - 0.2070*Temperature;
    apterous_potential=9.1917 - 0.3050*Temperature;}

    //CROP EFFECT ON FECUNDITY
    if(Wheat_Pheno>=59 && Wheat_Pheno<=73)
    {alate_potential=1.6*alate_potential;
    apterous_potential=1.6*apterous_potential;}

    //Maybe a bit abrupt: switches directly to 0 after GS80. Not what Plantegenest et al. 2001 did
    if(Wheat_Pheno>80) {alate_potential=
                        apterous_potential=0;}

    //FOR NOW NO MORPH DETERMINATION
    if(alate_potential<0){alate_potential=0;}
    if(apterous_potential<0){apterous_potential=0;}// there is a problem at GS73, the fecundity is <0.

}


}
