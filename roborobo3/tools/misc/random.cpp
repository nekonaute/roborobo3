// clang++ -std=c++11 -stdlib=libc++ -Weverything random.cpp

#include <iostream>
#include <random>
#include <chrono>

int main () //int argc, char *argv[])
{
	std:: minstd_rand randint;

    	randint.seed(0);

    	for (int i = 0; i < 10; i++)
        	std::cout << randint() << '\n';
}

