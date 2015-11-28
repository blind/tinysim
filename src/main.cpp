#include <SDL2/SDL.h>
#include "tinysim.h"

#include <cstdio>

int main(int argc, char**argv)
{
	auto result = InitTinySim();
	printf("result: %d\n", result);
	return 0;
}


