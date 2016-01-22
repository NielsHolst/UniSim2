#include "publish.h"

namespace BOXES_PLUGIN_NAME {

base::ProductList& productList() {
    static base::ProductList theList;
    return theList;
}

} //namespace

