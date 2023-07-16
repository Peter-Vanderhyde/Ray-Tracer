#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <tuple>
#include "lodepng.h"

using namespace std;

//Function to read the RGB values of a given png image
std::tuple<vector<unsigned char>, int, int> readPNGImage(const string& fileName);

//Function to calculate the average RGB values for each pixel
vector<unsigned char> averageRGBValues(const vector<vector<unsigned char>>& imagesVec);

//Function to save the new image as a PNG
void saveAsPNGImage(const vector<unsigned char>& averageRGB, const string& fileName, int width, int height);

//Function to read all PNG images in the given directory and create a new image with the average RGB values
void createAverageImage(const string& directoryName, const string& fileName);