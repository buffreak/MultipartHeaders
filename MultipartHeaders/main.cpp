#include "Multipart.cpp"
int main(int argc, char** argv) {

	std::map<std::string, std::string> mime{
		{"param1", "value1"},
		{"param2", "digidaw.txt"},
	};

	std::vector<std::string> contentEncoding{
		std::string(), // not encoding for param1 (not encode anything because init to empty string)
		"base64", // encode content from param2 with base64 encoding
	};

	try {
		Multipart multipart;
		multipart.createHeader(mime, contentEncoding);

		//or if tou dont want to encode content
		//multipart.createHeader(mime, std::vector<std::string>());

		std::string multipartHeader = multipart.create();
		std::cout << multipartHeader;
	}
	catch (std::exception& e) {
		std::cout << e.what();
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return EXIT_SUCCESS;
}