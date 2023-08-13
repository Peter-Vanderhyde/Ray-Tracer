#pragma once

#include <vector>
#include <bitset>
#include <optional>

using byte = std::bitset<8>;

std::optional<std::vector<std::bitset<8>>> get_details(const std::string& input_file, const int max_bits_possible);
void save_details(const std::string& image_file, const std::optional<std::vector<std::bitset<8>>>& scene_details);
std::optional<std::vector<char>> extract_details(const std::string &image_file);