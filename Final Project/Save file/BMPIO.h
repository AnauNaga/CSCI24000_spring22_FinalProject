#ifndef BMPIO_H_INCLUDED
#define BMPIO_H_INCLUDED
#include <vector>


class BMPIO {
public:
	BMPIO();
	~BMPIO();
	unsigned int getColorHex(int x, int y) const;
	void setColor(unsigned int color, int x, int y);
	int getHeight();
	int getWidth();
	
	
protected:
	int image_width;
	int image_height;
	void* image_buffer;
};


#endif
