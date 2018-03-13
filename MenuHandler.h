#pragma once
#include "OffModel.h"

class MenuHandler {


public: 
	
	int rotation_hearthbeat;


	//functions
	MenuHandler(void);
	~MenuHandler(void);
	void setOffObj(OffModel*);
	void setProgram(GLuint);
	void setRotHB(int);
	void setGlobalMenuHandler(MenuHandler*);

};