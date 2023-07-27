#ifndef _VECTOR_H
#define _VECTOR_H

struct vector {
    int x;
    int y;
};
struct vector add_vector(struct vector a, struct vector b);
struct vector scala_vector(struct vector a, int n);
void printv(struct vector v);

#endif //_VECTOR_H
