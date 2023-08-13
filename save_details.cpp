#include "save_details.h"
#include <vector>
#include <bitset>
#include <fstream>
#include "lodepng.h"
#include <iostream>
#include <optional>
#include <sstream>

// Set the least significant bit of each rgb value to 0
std::vector<byte> wipe_bits(const std::vector<unsigned char>& image) {
	std::vector<byte> bytes;
	for (int c : image) {
		// Convert the color value to an int and then store as binary in vector
		byte b = byte(c);
		b.set(0, 0);
		bytes.push_back(b);
	}
	return bytes;
}

// Extract the chars from the input file, convert each to binary, then store in vector and return
std::optional<std::vector<byte>> get_details(const std::string& input_file, const int max_bits_possible) {
	std::ifstream input(input_file);
	if (!input) {
		throw std::runtime_error("Cannot open input file: " + input_file);
	}
	std::string line;
	std::vector<byte> bits;
	while (std::getline(input, line)) {
		line += '\n';
		for (int c : line) {
			bits.push_back(byte(c));
		}
	}

	if (static_cast<int>(bits.size()) * 8 <= max_bits_possible) {
		return bits;
	} else {
		return {};
	}
}

// Edit the least significant bit of each rgb value to be a bit from the input details
void set_colors(std::vector<byte>& colors, const std::vector<byte>& details) {
	// Set the bottom bit to each bit of the chars
	for (int i = 0; i < static_cast<int>(details.size()); ++i) {
		for (int j = 0; j < 8; ++j) {
			colors[i * 8 + j].set(0, details[i].test(j));
		}
	}
}

// Change the values in the image vector then save that as the old image again
void save_new_image(std::vector<unsigned char>& image, const std::vector<byte>& colors) {
	for (int i = 0; i < static_cast<int>(image.size()); ++i) {
		image[i] = colors[i].to_ulong();
	}
}

// Save the input values in the image itself so you know what values made the images
void save_details(const std::string& image_file, const std::optional<std::vector<byte>>& scene_details) {
	if (!scene_details.has_value()) {
		std::cout << "Unable to save details in image. Image not big enough.\n";
		return;
	}

	std::ifstream input(image_file);
	if(!input) {
		throw std::runtime_error("Cannot open image file: " + image_file);
	}
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, image_file);
	if (error) {
		std::cout << "Error: " + error;
	}

	std::vector<byte> colors{wipe_bits(image)};
	set_colors(colors, scene_details.value());
	save_new_image(image, colors);
	error = lodepng::encode(image_file, image, width, height);
	if (error) {
		throw std::runtime_error(lodepng_error_text(error));
	}
	std::cout << "Successfully saved properties of image.\n";
}

std::optional<std::vector<char>> extract_details(const std::string& image_file) {
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, image_file);
	if (error) {
		std::cout << "Error: Unable to read from file.\nMake sure the whole path was specified from the current directory.";
		return {};
	}

	std::vector<char> details;
	byte c;
	c.reset();
	for (int i = 0; i < static_cast<int>(image.size()); ++i) {
		c.set(i % 8, byte(image[i]).test(0));
		if ((i + 1) % 8 == 0) {
			if (c.to_ulong() == 0) {
				break;
			}

			details.push_back(static_cast<char>(c.to_ulong()));
		}
	}

	return details;
}