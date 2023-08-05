#include <iostream>
#include <fstream>
#include "save_details.h"



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Missing image name.";
        return 0;
    }

    std::string scene_filename{""};
    if (argc == 3) {
        scene_filename = argv[2];
    }

    std::string image_name = argv[1];

    std::vector<char> details;
    details = extract_details(image_name);
    if (scene_filename != "") {
        std::ofstream file{scene_filename};
        for (char c : details) {
            file << c;
        }
    }
    else {
        for (char c : details) {
            std::cout << c;
        }
    }
}