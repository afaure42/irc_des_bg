#include "User.hpp"
// #include "Channel.hpp"

int main() {
	// User	user_test("test");
	User	user_test2("test2");

	user_test2.check();
	std::cout << user_test2;
	user_test2.setAwayStatus();
	std::cout << user_test2;
}