#include "lodepng.h"
#include <iostream>
#include <bitset>

using byte = std::bitset<8>;

void extract_details(const std::string& image_file) {
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, image_file);
	if (error) {
		std::cout << "Error: " + error;
	}

	byte c;
	c.reset();
	for (int i = 0; i < image.size(); ++i) {
		c.set(i % 8, byte(image[i]).test(0));
		if ((i + 1) % 8 == 0) {
			if (c.to_ulong() == 0) {
				break;
			}
			std::cout << (char)c.to_ulong();
		}
	}
}

int main() {
    std::string filename;
    std::cout << "What is the image file? ";
    std::cin >> filename;
    extract_details(filename);
}