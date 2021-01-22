#include <QVector>
#include <base/data_frame.h>
#include <base/publish.h>
#include "data_frame_input.h"

using namespace base;

namespace test_boxes {

PUBLISH(DataFrameInput)

//bool isPath(QString s) {

//}
//#define InputVar(X,Y) (*new Port(X, this)).data(& Y).access(PortAccess::Input)

DataFrameInput::DataFrameInput(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(defaultValue).equals(17);
    Input(dfFileName).equals("data_frame.txt");
}

void DataFrameInput::amend() {
//    DataFrame df;
//    df.read(dfFileName, DataFrame::BothLabelled);
//    matrix.copySizeFrom(df);

//    for (int row=0; row<df.numRow(); ++row) {
//        for (int col=0; col<df.numCol(); ++col) {

//            QString name = df.rowNames().at(row) + "_" + df.colNames().at(col);
//            InputVar(name).from(df(row,col)).into(matrix(row,col));
//        }

//    }
}

}
