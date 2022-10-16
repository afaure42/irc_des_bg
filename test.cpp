#include "Channel.hpp"
#include "common.hpp"

int main()
{
	Channel test("test");

	std::cout << test.getName() << std::endl;
	std::cout << test(test);
}