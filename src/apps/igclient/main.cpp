#define IGLIB_DLL dllimport

#include <iostream>
#include <iglib/iglib.h>

using namespace ig;
using namespace std;


int main(int , char **)
{
    Query q = randomQuery();
    Response r = compute(q);

    if (r.hasError) {
        cout << "\n\nError in iglib:\n" << r.error << "\n";
    }
    else
        cout << responseToString(r);

    ig::release();
    return 0;
}
