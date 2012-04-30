#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "qt.h"



class OnlyOne
{
public:
	static OnlyOne& const Instance()
	{
		static OnlyOne theSingleInstance;
		return theSingleInstance;
	}
private:        
	OnlyOne(){}
	OnlyOne(OnlyOne& root){}
	OnlyOne& operator=(OnlyOne&){}
};


#endif