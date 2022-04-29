#include "BMPIN.h"
#include "BMPIO.h"
#include <fstream>
#include <iostream>


BMPIN::BMPIN() : BMPIO() {
	image_buffer = 0;
	image_width = 0;
}

BMPIN::~BMPIN() {
	delete image_buffer;
}

int BMPIN::Import(const char* path, unsigned int exclude){
	std::ifstream imageFile;
	imageFile.open(path, std::ios::in | std::ios::binary);

	if (!imageFile.is_open()) {
		std::cout << "file failed to open" << std::endl;
		return 1;
	}

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;


	unsigned char informationHeader[informationHeaderSize] = {};
	unsigned char fileHeader[fileHeaderSize] = {};

	imageFile.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	imageFile.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	/*
	for (int i = 0; i < fileHeaderSize; i++) {
		imageFile.read(nextData, 1);
		fileHeader[i] = reinterpret_cast<unsigned char>(nextData);
	}
	for (int i = 0; i < informationHeaderSize; i++) {
		imageFile.read(nextData, 1);
		informationHeader[i] = reinterpret_cast<unsigned char>(nextData);
	}
	
	*/
	int fileSize;
	//file size
	fileSize = static_cast<int>(fileHeader[2]) + static_cast<int>(fileHeader[3] << 8) + static_cast<int>(fileHeader[4] << 16) + static_cast<int>(fileHeader[5] << 24);
	

	// image width;
	setWidth(static_cast<int>(informationHeader[4]) + static_cast<int>(informationHeader[5] << 8) + static_cast<int>(informationHeader[6] << 16) + static_cast<int>(informationHeader[7] << 24));
	//image height
	setHeight(static_cast<int>(informationHeader[8]) + static_cast<int>(informationHeader[9] << 8) + static_cast<int>(informationHeader[10] << 16) + static_cast<int>(informationHeader[11] << 24));
	
	image_buffer = malloc(image_width * image_height * 4);
	// planes
	//informationHeader[12] = 1;
	//informationHeader[13] = 0;
	// bits per pixel
	const char BitsPerPixel = informationHeader[14];
	int BytesPerPixel = BitsPerPixel / 8;


	unsigned char bmpPadding[3] = { 0,0,0 };
	const int paddingAmount = ((4 - (BMPIN::image_width * 3) % 4) % 4);
	unsigned int* pixel;
	char paddingdump;
	char* padding = &paddingdump;

	unsigned char* nextData = new unsigned char[BytesPerPixel];
	for (int y = 0; y < image_height; y++) {
		for (int x = 0; x < image_width; x++) {
			imageFile.read(reinterpret_cast<char*>(nextData), BytesPerPixel);
			pixel = ((unsigned int*)image_buffer + x + y * image_width);
			*pixel = 0;
			for (int i = 0; i < BytesPerPixel; i++) {
				*pixel = *pixel + (nextData[i] << (i * 8));
			}
			if (*pixel == exclude) {
				*pixel = *pixel + 0xFF000000;
			}
		}
		for (int i = 0; i < paddingAmount; i++) {
			imageFile.read(padding,1);
		}
	}
	delete[] nextData;
	imageFile.close();
	return 0;
}

void BMPIN::setWidth(int newWidth){
	BMPIN::image_width = newWidth;
}

void BMPIN::setHeight(int newHeight){
	BMPIN::image_height = newHeight;
}

