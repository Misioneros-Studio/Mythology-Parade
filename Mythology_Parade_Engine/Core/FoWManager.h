#ifndef __FOW_MANAGER_H__
#define __FOW_MANAGER_H__
#include "SDL_image/include/SDL_image.h"
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include "p2Point.h"
#include "FoWBitDefs.h"
#include "j1Module.h"
//#include "FoWEntity.h"


//this struct holds the information of fog and shroud for every tile
struct FoWDataStruct
{
	unsigned short tileFogBits; //saves information about which type of fog are we in (useful for smooth edges)
	unsigned short tileShroudBits; //same as above but for shroud
};

struct MaskData
{
	int numberOfUsers = 0;
	unsigned short* mask = nullptr;
};


class FoWManager :public j1Module
{
	//FUNCTIONS
public:
	FoWManager();
	~FoWManager();


	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	//FoWEntity* CreateFoWEntity(iPoint pos, bool providesVisibility);

	//Resets the map to its shrouded state
	void ResetFoWMap();
	void CreateFoWMap(uint width, uint height);
	void DeleteFoWMap();
	//Updates the data on the FoWMap based on the FoWEntities position and mask shape
	void UpdateFoWMap();
	void DrawFoWMap();
	//Tell the map that it needs to be updated the next frame
	void MapNeedsUpdate();

	//Returns the visibility state of the chosen tile (given its map coordinates)
	FoWDataStruct* GetFoWTileState(iPoint mapPos)const;
	//Returns true if the tile is inside the map boundaries, otherwise returns false
	bool CheckFoWTileBoundaries(iPoint mapPos)const;
	//Returns true if the tile is visible (there's no FOG in it) otherwise returns false
	bool CheckTileVisibility(iPoint mapPos)const;
	//Returns true if the tile is visible  (there's no FOG in it) otherwise returns false (if there's shroud it will return true)
	bool CheckTileVisibilityWithoutCountingShroud(iPoint mapPos)const;
	//Returns true if the tile has shroud instead of fog (only use when certain tile is not visible)
	bool CheckShroud(iPoint mapPos)const;


	unsigned short* GetMaskFromRadius(int radius);
	void RequestMaskGeneration(int radius);
	void RequestMaskDeletion(int radius);

	bool InsideCircle(iPoint center, iPoint tile, float radius);


	void GetTilesInsideRadius(int fowRadius, fPoint position, iPoint offSet, std::vector<iPoint>& ret)const;
	void ApplyMaskToTiles(int fowRadius, std::vector<iPoint>& tilesAffected);

private:
	unsigned short* GenerateCircle(int radius);
	unsigned short* GenerateCircleBorders(int radius, unsigned short* mask);
	unsigned short* GenerateCircleJoints(int radius, unsigned short* mask);
	unsigned short CheckCornersFromNeighbours(iPoint pos, int diameter, unsigned short* mask);
	unsigned short CheckJointsFromNeighbours(iPoint pos, int diameter, unsigned short* mask);


private:
	//This is where the FoWEntites are stored
	//std::vector<FoWEntity*> fowEntities;
	//This is where we store our FoW information
	FoWDataStruct* fowMap = nullptr;
	std::map<uint, MaskData> maskMap;

	SDL_Texture* smoothFoWtexture = nullptr;
	SDL_Texture* debugFoWtexture = nullptr;

	//Map that we use to translate bits to Texture Id's
	std::map<unsigned short, uint> bitToTextureTable;

	uint width;
	uint height;
	bool debugMode = false;
	bool foWMapNeedsRefresh = false;
};

#endif // !__FOW_MANAGER_H__