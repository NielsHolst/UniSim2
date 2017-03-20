#define IGLIB_DLL dllimport

#include <iostream>
#include <string>
#include <iglib/iglib.h>

using namespace ig;

int main(int , char **)
{
    Query q = randomQuery();
    Response r = compute(q);
    std::cout << queryToString(q) << "\n";
    std::cout << responseToString(r) << "\n";
    ig::release();
    return 0;
}
