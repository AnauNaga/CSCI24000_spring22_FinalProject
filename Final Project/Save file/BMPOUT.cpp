#include "BMPOUT.h"
#include <fstream>
#include <iostream>
#include <vector>


BMPOUT::BMPOUT(int width, int height) : BMPIO() {
	BMPOUT::image_width = width;
	BMPOUT::image_height = height;
	BMPOUT::image_buffer = malloc(width * height * 4);
}

BMPOUT::~BMPOUT() {
	delete image_buffer;
}

void BMPOUT::Export(const char* path) const{
	std::ofstream imageFile;
	imageFile.open(path, std::ios::out | std::ios::binary);

	if (!imageFile.is_open()) {
		std::cout << "file failed to open" << std::endl;
		return;
	}

	unsigned char bmpPadding[3] = { 0,0,0 };
	const int paddingAmount = ((4 - (BMPOUT::image_width * 3) % 4) % 4);

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int fileSize = (fileHeaderSize + informationHeaderSize + image_width * image_height * 3 + paddingAmount * image_height);

	unsigned char fileHeader[fileHeaderSize];

	//file type
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	//file size
	fileHeader[2] = fileSize;
	fileHeader[3] = fileSize >> 8;
	fileHeader[4] = fileSize >> 16;
	fileHeader[5] = fileSize >> 24;
	//reserved 1
	fileHeader[6] = 0;
	fileHeader[7] = 0;
	//reserved 2
	fileHeader[8] = 0;
	fileHeader[9] = 0;
	//pixel data offset
	fileHeader[10] = fileHeaderSize + informationHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];

	// header Size
	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;
	// image width;
	informationHeader[4] = image_width;
	informationHeader[5] = image_width >> 8;
	informationHeader[6] = image_width >> 16;
	informationHeader[7] = image_width >> 24;
	//image height
	informationHeader[8] = image_height;
	informationHeader[9] = image_height >> 8;
	informationHeader[10] = image_height >> 16;
	informationHeader[11] = image_height >> 24;
	// planes
	informationHeader[12] = 1;
	informationHeader[13] = 0;
	// bits per pixel
	informationHeader[14] = 32;
	informationHeader[15] = 0;
	// compression?
	informationHeader[16] = 0;
	informationHeader[17] = 0;
	informationHeader[18] = 0;
	informationHeader[19] = 0;
	// image size with compression
	informationHeader[20] = 0;
	informationHeader[21] = 0;
	informationHeader[22] = 0;
	informationHeader[23] = 0;
	//x pixels per meter
	informationHeader[24] = 0;
	informationHeader[25] = 0;
	informationHeader[26] = 0;
	informationHeader[27] = 0;
	//y pixels per meter
	informationHeader[28] = 0;
	informationHeader[29] = 0;
	informationHeader[30] = 0;
	informationHeader[31] = 0;
	// total colors for a color pallet
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;
	//important colors
	informationHeader[36] = 0;
	informationHeader[37] = 0;
	informationHeader[38] = 0;
	informationHeader[39] = 0;

	imageFile.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	imageFile.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	
	unsigned int color;
	for (int y = 0; y < image_height; y++) {
		for (int x = 0; x < image_width; x++) {
			color = *((unsigned int*)image_buffer + x + y * image_width);
			imageFile.write(reinterpret_cast<char*>(&color), 4);

		}
		imageFile.write(reinterpret_cast<char*>(bmpPadding), paddingAmount);
	}

	imageFile.close();
}
