#pragma once

#include <vector>
#include <bitset>
#include <optional>

using byte = std::bitset<8>;

void save_details(const std::string& image_file, const std::string& input_file, const int max_bits_possible);
void extract_details(const std::string &image_file);