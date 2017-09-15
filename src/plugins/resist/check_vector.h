#ifndef CHECK_VECTOR_H
#define CHECK_VECTOR_H

#define CHECK_VECTOR_3(v) \
    if (v.size() != 3) \
        ThrowException(#v "vector not of length 3").value(v.size());

#endif

