#include <iostream>
#include <exception>

class MyException {
public:
	MyException(const std::string error_type) {
		error = error_type;
	}
	~MyException() {}
	std::string what() {
		return error;
	}
private:
	std::string error;
};