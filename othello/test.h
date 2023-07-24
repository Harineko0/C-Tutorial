#ifndef _TEST_H_
#define _TEST_H_
#include "hull.h"

void expectULL(char* testName, ull input, ull expect);
void expectHULL(char* testName, hull input, hull expect, int size);

#endif // _TEST_H_