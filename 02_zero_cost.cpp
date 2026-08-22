#include<iostream>

// High-level abstraction: inline template function
template <typename T>
inline T multiply_by_two(T val) {
	return val * 2;
}

int main() {
	int x = 10;
	int result = multiply_by_two(x);
	std::cout << "Result: " << result << "\n";
	return 0;
}
