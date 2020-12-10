#include <base/publish.h>
#include "structure.h"

using namespace base;

namespace saccharina {

PUBLISH(Structure)

Structure::Structure(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps track of structural mass");
    Input(proportionC).equals(0.20).unit("[0;1]").help("Proportion of the structural mass that is carbon");
    Input(proportionN).equals(0.01).unit("[0;1]").help("Proportion of the structural mass that is nitrogen");
    Output(mass).imports("./ageing[content]").unit("g").help("Structural mass");
    Output(senescent).imports("./ageing[outflowTotal]").unit("g").help("Cumulative senescent structural mass");
    Output(respired).imports("./respired[value]").unit("g").help("Cumulative respired structural mass");
}

}
