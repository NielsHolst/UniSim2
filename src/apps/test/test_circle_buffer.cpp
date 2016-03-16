#include <iostream>
#include <boost/circular_buffer.hpp>
#include "test_circle_buffer.h"

void TestCircleBuffer::testIndices() {
    // Create a circular buffer with a capacity for 3 integers.
    boost::circular_buffer<int> cb(3);

    // Insert threee elements into the buffer.
    cb.push_back(1);
    cb.push_back(2);
    std::cout << "\nA\n";
    for (auto it : cb) std::cout << it << "\n";
    cb.push_back(3);

    int a = cb[0];  // a == 1
    int b = cb[1];  // b == 2
    int c = cb[2];  // c == 3
    QCOMPARE(a, 1);
    QCOMPARE(b, 2);
    QCOMPARE(c, 3);

    // The buffer is full now, so pushing subsequent
    // elements will overwrite the front-most elements.

    cb.push_back(4);  // Overwrite 1 with 4.
    cb.push_back(5);  // Overwrite 2 with 5.

    // The buffer now contains 3, 4 and 5.
    a = cb[0];  // a == 3
    b = cb[1];  // b == 4
    c = cb[2];  // c == 5
    QCOMPARE(a, 3);
    QCOMPARE(b, 4);
    QCOMPARE(c, 5);

    std::cout << "\nB\n";
    for (auto it : cb) std::cout << it << "\n";

}
