#ifndef __j1Entities_H__
#define __j1Entities_H__

#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2DynArray.h"
#include "p2Point.h"
#define MAX_ENTITIES 100

enum ENTITY_TYPES
{
	NO_TYPE,
	PLAYER,
};

class Entity;

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
	int x, y;
};

class j1Entities : public j1Module
{
public:

	j1Entities();
	~j1Entities();

	bool Start();
	bool PreUpdate();
	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool AddEntity(ENTITY_TYPES type, int x, int y);


private:

	void SpawnEntity(const EntityInfo& info);

private:
	EntityInfo queue[MAX_ENTITIES];
	Entity* entities[MAX_ENTITIES];

	p2DynArray<iPoint> Positions;

public:
	iPoint player_pos;
	bool justloaded = false;
	SDL_Texture* atlas = nullptr;
};
#endif
