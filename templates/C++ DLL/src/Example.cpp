#include "Math.h"

int example(int a, int b) {
	return a + b;
}

#ifdef DLL
int main(void) {
    return 0;
}
#endif

