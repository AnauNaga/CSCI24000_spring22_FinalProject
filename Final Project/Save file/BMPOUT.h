#ifndef BMPOUT_H_INCLUDED
#define BMPOUT_H_INCLUDED
#include "BMPIO.h"
class BMPOUT : public BMPIO {
public:
	BMPOUT(int width, int height);
	~BMPOUT();
	void Export(const char* path) const;

};
#endif