#include <base/publish.h>
#include "r0_Schmitz.h"

using namespace base;

namespace aphid {

PUBLISH(R0_Schmitz)

R0_Schmitz::R0_Schmitz(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Tmin);
    Input(Tmax);
    Input(Topt);
    Input(R0_opt);
    Input(Temperature);
    Input(Wheat_Pheno);
    Output(lifetime_fec_apterous);
    Output(lifetime_fec_alates);
} 
void R0_Schmitz::reset() {
    lifetime_fec_apterous=
    lifetime_fec_alates=0;
}

void R0_Schmitz::update() {
    // TEMPERATURE EFFECT ON LIFETIME FECUNDITY
    //first equation's parameters from Tmin to Topt
    if (Temperature>=Tmin && Temperature<=Topt)
       {double a=R0_opt/(Topt-Tmin); //local variable that exists only here. Avoid mistakes.
        double b=(-Tmin*R0_opt)/(Topt-Tmin);
        lifetime_fec_apterous=a*Temperature+b;
        }
    //second equation's parameters from Topt to Tmax
    if (Temperature>Topt && Temperature<=Tmax)
       {double a=R0_opt/(Topt-Tmax);
        double b=(-Tmax*R0_opt)/(Topt-Tmax);
        lifetime_fec_apterous=a*Temperature+b;
        }

    // WHEAT PHENOLOGY EFFECT ON LIFETIME FECUNBITY
    if(Wheat_Pheno>=59 && Wheat_Pheno<=73)
    {lifetime_fec_apterous=1.6*lifetime_fec_apterous;}

    if(Wheat_Pheno>80) {lifetime_fec_apterous=0;}

    // THE LIMITS SITUATIONS CLOSE TO Tmin AND Tmax
    if(lifetime_fec_apterous<0) {lifetime_fec_apterous=0;}

    // THE CASE OF THE ALATES NOW
    // In Duffy et al. 2017 the maximum daily nymph production per alates=2/3 of the max fec of apterous
    lifetime_fec_alates=(lifetime_fec_apterous*2)/3;
}


}
