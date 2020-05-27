#ifndef _IA_
#define _IA_

#include "p2Point.h"
#include "j1Module.h"
#include "PugiXml/src/pugixml.hpp"
#include"j1Input.h"
#include "EntityManager.h"


class IA : public j1Module
{
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

};


#endif // !_IA_
