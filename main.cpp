#include<iostream>

int main() {
	int number = 42;
	int* ptr = &number; // Pointer storing the memory address of 'number'

	std::cout << "================ HARDWARE DEMO ================\n";
	std::cout << "Value of 'number': " << number << "\n";
	std::cout << "Size of 'number' in RAM: " << sizeof(number) << " bytes\n";
	std::cout << "Memory Address of 'number': " << ptr << "\n";
	std::cout << "Size of Pointer itself: " << sizeof(ptr) << " bytes\n";
	std::cout << "===============================================\n";

	return 0;
}
