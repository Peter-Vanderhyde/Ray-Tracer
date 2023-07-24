#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include "lodepng.h"
#include "combine_images.h"

using namespace std;

//Function to read the RGB values of a given png image
std::tuple<vector<unsigned char>, int, int> readPNGImage(const string& fileName) {
	vector<unsigned char> image; //The raw pixels
	unsigned width, height;
	
	//Decode the PNG file using lodepng
	unsigned error = lodepng::decode(image, width, height, fileName);
	
	//Error checking
	if(error){
        cout << "Decoder error " << error << ": " << lodepng_error_text(error) << endl;
    }
	
	return std::make_tuple(image, width, height);
}

//Function to calculate the average RGB values for each pixel
vector<unsigned char> averageRGBValues(const vector<vector<unsigned char>>& imagesVec) {
	vector<unsigned char> averageRGB;
	int numImages = imagesVec.size();
	
	//Iterate through each pixel
	for (int i = 0; static_cast<unsigned int>(i) < imagesVec[0].size(); i += 4) {
		int redSum = 0;
		int greenSum = 0;
		int blueSum = 0;
		
		//Get the sum of the red, green and blue values of each pixel
		for (int j = 0; j < numImages; j++) {
			redSum += imagesVec[j][i];
			greenSum += imagesVec[j][i + 1];
			blueSum += imagesVec[j][i + 2];
		}
		
		//Calculate the average values
		unsigned char redAvg = static_cast<unsigned char>(redSum / numImages);
		unsigned char greenAvg = static_cast<unsigned char>(greenSum / numImages);
		unsigned char blueAvg = static_cast<unsigned char>(blueSum / numImages);
		
		//Add the values to the result vector
		averageRGB.push_back(redAvg);
		averageRGB.push_back(greenAvg);
		averageRGB.push_back(blueAvg);
		averageRGB.push_back(255); //Add the alpha value to the vector
	}
	
	return averageRGB;
}

//Function to save the new image as a PNG
void saveAsPNGImage(const vector<unsigned char>& averageRGB, const string& fileName, int width, int height) {
	
	//Encode and save the image as a PNG
	unsigned error = lodepng::encode(fileName, averageRGB, width, height);
	
	//Error checking
	if (error) cout << "Encoder error " << error << ": " << lodepng_error_text(error) << endl;
}

//Function to read all PNG images in the given directory and create a new image with the average RGB values
void createAverageImage(const string& directoryName, const string& fileName) {
	vector<vector<unsigned char>> imageVec; //Vector of image data
	
	//Open the directory
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(directoryName.c_str())) != NULL) {
		int width = 0, height = 0;
		//Loop through the directory
		while ((ent = readdir(dir)) != NULL) {
			string file(ent->d_name); //Get the file name
			
			//Check if the file is a PNG image
			if(file.length() > 4 && file.substr(file.length() - 4) == ".png") {
				//Read the image data
				vector<unsigned char> image;
				auto result = readPNGImage(directoryName + "/" + file);
				image = std::get<0>(result);
				width = std::get<1>(result);
				height = std::get<2>(result);
				
				//Add it to the vector
				imageVec.push_back(image);
			}
		}
		
		//Close the directory
		closedir(dir);
		
		//Calculate the average RGB values
		vector<unsigned char> averageRGB = averageRGBValues(imageVec);
		
		//Save the image as a PNG
		saveAsPNGImage(averageRGB, fileName, width, height);
	}
	else {
		//Error: Could not open the directory
		cerr << "Error: Could not open directory \"" << directoryName << "\"" << endl;
	}
}

// int main() {
// 	string directoryName = "files/checkpoints";
// 	string fileName = "files/renders/combined.png";
// 	int width, height;
// 	cout << "What is the width? ";
// 	cin >> width;
// 	cout << "What is the height? ";
// 	cin >> height;
// 	createAverageImage(directoryName, fileName, width, height);
	
// 	return 0;
// }