#ifndef BMPIN_H_INCLUDED
#define BMPIN_H_INCLUDED
#include "BMPIO.h"

class BMPIN : public BMPIO {
public:
	BMPIN();
	~BMPIN();
	int Import(const char* path, unsigned int exclude = 0xFFFFFFFF);
	void setWidth(int newWidth);
	void setHeight(int newHeight);
private:
};



#endif
