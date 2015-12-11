#include "publish.h"

namespace BOXES_PLUGIN_NAME {

boxes::ProductList& productList() {
    static boxes::ProductList theList;
    return theList;
}

} //namespace

