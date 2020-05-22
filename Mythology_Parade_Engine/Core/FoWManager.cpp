#include "FoWManager.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Input.h"

FoWManager::FoWManager()
{
}


FoWManager::~FoWManager()
{
	CleanUp();
}


bool FoWManager::Awake(pugi::xml_node&)
{
	bool ret = true;
	active = false;
	return ret;
}


bool FoWManager::Start()
{
	bool ret = true;

	smoothFoWtexture = App->tex->Load("maps/fogTiles.png");
	debugFoWtexture = App->tex->Load("maps/fogTilesDebug.png");

	if (smoothFoWtexture == nullptr || debugFoWtexture == nullptr);
	ret = false;

	//---------Initialize the map being used to translate bits to texture ID---------//
	//Straight-forward cases
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_ALL, 0));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_NNN, 1));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_WWW, 2));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_EEE, 3));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_SSS, 4));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CNW, 5));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CSE, 6));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CNE, 7));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CSW, 8));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JNE, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JSW, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JNW, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JSE, 12));

	//more complicated cases (combinations)
	//diagonals
	bitToTextureTable.insert(std::pair<unsigned short, int>(20, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(80, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(17, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(272, 12));
	bitToTextureTable.insert(std::pair<unsigned short, int>(273, 13));
	bitToTextureTable.insert(std::pair<unsigned short, int>(84, 14));
	//lines
	bitToTextureTable.insert(std::pair<unsigned short, int>(23, 1));
	bitToTextureTable.insert(std::pair<unsigned short, int>(308, 3));
	bitToTextureTable.insert(std::pair<unsigned short, int>(89, 2));
	bitToTextureTable.insert(std::pair<unsigned short, int>(464, 4));
	//joints
	bitToTextureTable.insert(std::pair<unsigned short, int>(6, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(36, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(72, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(192, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(3, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(9, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(384, 12));
	bitToTextureTable.insert(std::pair<unsigned short, int>(288, 12));
	//corners
	bitToTextureTable.insert(std::pair<unsigned short, int>(4, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(64, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(1, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(256, 12));
	//------------------------end of map initialization------------------------//



	return ret;
}


bool FoWManager::PreUpdate()
{
	bool ret = true;

	//deletes all the entities that request to do so
	//for (int i = 0; i < fowEntities.size(); i++)
	//{
	//	if (fowEntities[i]->deleteEntity)
	//	{
	//		delete fowEntities[i];
	//		fowEntities[i] = nullptr;
	//		fowEntities.erase(fowEntities.begin() + i);
	//		i--;
	//	}

	//}

	//debug input handling
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		ResetFoWMap();
		MapNeedsUpdate();
	}
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debugMode = !debugMode;
		MapNeedsUpdate();
	}

	return ret;
}


bool FoWManager::Update(float dt)
{
	bool ret = true;

	//We update the fowMap only when its needed
	if (foWMapNeedsRefresh)
	{
		UpdateFoWMap();
		foWMapNeedsRefresh = false;
	}
	return ret;
}


bool FoWManager::PostUpdate()
{
	bool ret = true;
	DrawFoWMap();
	return ret;
}


bool FoWManager::CleanUp()
{
	bool ret = true;
	DeleteFoWMap();

	int i = 0;
	//while (fowEntities.size() > 0)
	//{


	//	if (fowEntities[i] != nullptr)
	//	{
	//		delete fowEntities[i];
	//		fowEntities[i] = nullptr;
	//		fowEntities.erase(fowEntities.begin() + i);
	//		i--;
	//	}
	//	i++;
	//}
	//fowEntities.clear();


	if (debugFoWtexture != nullptr)
	{
		App->tex->UnLoad(debugFoWtexture);
		debugFoWtexture = nullptr;
	}
	if (smoothFoWtexture != nullptr)
	{
		App->tex->UnLoad(smoothFoWtexture);
		smoothFoWtexture = nullptr;
	}

	return ret;
}


void FoWManager::ResetFoWMap()
{
	if (fowMap != nullptr)
	{
		memset(fowMap, NULL, width * height);

		for (int i = 0; i < width * height; i++)
		{
			fowMap[i].tileShroudBits = fow_ALL;
			fowMap[i].tileFogBits = fow_ALL;
		}
	}
}


FoWDataStruct* FoWManager::GetFoWTileState(iPoint mapPos)const
{
	FoWDataStruct* ret = nullptr;
	if (CheckFoWTileBoundaries(mapPos) && fowMap != nullptr)
	{
		ret = &fowMap[(mapPos.y * width) + mapPos.x];
	}

	return ret;
}


bool FoWManager::CheckFoWTileBoundaries(iPoint mapPos)const
{
	bool ret = false;
	if (mapPos.x >= 0 && mapPos.x < width && mapPos.y >= 0 && mapPos.y < height)
		ret = true;

	return ret;
}

void FoWManager::CreateFoWMap(uint w, uint h)
{
	width = w;
	height = h;

	//TODO 1: Complete this function to create a FoWMap. EASY!
	//If a map has already been created you will need to delete it first, hint: there's a function for that :) 
	//Note that the map will be a 1 dimensional array and you might need the 2 variables above to set it up. The map should be stored in the variable "fowMap"
	//Don't forget to reset it once is creeated, hint: there's another function for that :)
	DeleteFoWMap();
	fowMap = new FoWDataStruct[width * height];
	ResetFoWMap();

	MapNeedsUpdate();
}


void FoWManager::DeleteFoWMap()
{
	if (fowMap != nullptr)
	{
		RELEASE_ARRAY(fowMap);
		fowMap = nullptr;
	}
}


void FoWManager::UpdateFoWMap()
{
	if (fowMap != nullptr)
	{
		for (int i = 0; i < width * height; i++)
		{
			fowMap[i].tileFogBits = fow_ALL;
		}

		//for (int i = 0; i < fowEntities.size(); i++)
		//{
		//	fowEntities[i]->Update();
		//}



		if (!debugMode)
		{
			//for (int i = 0; i < fowEntities.size(); i++)
			//{
			//	if (CheckTileVisibility(fowEntities[i]->GetPos()))
			//	{
			//		fowEntities[i]->isVisible = true;
			//	}
			//	else fowEntities[i]->isVisible = false;

			//}
		}
		else
		{
			//for (int i = 0; i < fowEntities.size(); i++)
			//{
			//	fowEntities[i]->isVisible = true;
			//}
		}


	}
}

void FoWManager::DrawFoWMap()
{

	j1PerfTimer timer;
	double oldT = timer.ReadMs();

	iPoint A = App->map->WorldToMap(-App->render->camera.x, -App->render->camera.y);
	iPoint B = App->map->WorldToMap(-App->render->camera.x + App->render->camera.w, -App->render->camera.y + App->render->camera.h);

	//Approach 2.0
	for (int a = A.x + A.y - 2; a <= B.x + B.y - 5 /* or 2*/; a++)
	{
		for (int b = A.x - A.y - 2; b <= B.x - B.y + 2; b++)
		{
			if ((b & 1) != (a & 1))
				continue;

			int x = (a + b) / 2;
			int y = (a - b) / 2;

			FoWDataStruct* tileInfo = GetFoWTileState({ x, y });
			int fogId = -1;
			int shroudId = -1;

			if (tileInfo != nullptr)
			{

				//This if's make the code go 2 times slower
				if (bitToTextureTable.find(tileInfo->tileFogBits) != bitToTextureTable.end())
				{
					fogId = bitToTextureTable[tileInfo->tileFogBits];
				}

				if (bitToTextureTable.find(tileInfo->tileShroudBits) != bitToTextureTable.end())
				{
					shroudId = bitToTextureTable[tileInfo->tileShroudBits];
				}
			}

			iPoint worldDrawPos;
			App->map->MapToWorld(x, y, worldDrawPos.x, worldDrawPos.y);

			SDL_Texture* displayFogTexture = nullptr;
			if (debugMode)
			{
				displayFogTexture = debugFoWtexture;
			}
			else displayFogTexture = smoothFoWtexture;

			//draw fog
			if (fogId != -1)
			{
				SDL_SetTextureAlphaMod(displayFogTexture, 128);//set the alpha of the texture to half to reproduce fog
				SDL_Rect r = { fogId * 64,0,64,64 }; //this rect crops the desired fog Id texture from the fogTiles spritesheet
				App->render->Blit(displayFogTexture, worldDrawPos.x, worldDrawPos.y - 16, &r);
				//App->render->Blit(displayFogTexture, worldDrawPos.x, worldDrawPos.y - 15, {64, 60}, &r, 1.0f);
			}
			if (shroudId != -1)
			{
				SDL_SetTextureAlphaMod(displayFogTexture, 255);//set the alpha to white again
				SDL_Rect r = { shroudId * 64,0,64,64 }; //this rect crops the desired fog Id texture from the fogTiles spritesheet
				App->render->Blit(displayFogTexture, worldDrawPos.x, worldDrawPos.y - 16, &r);
				//App->render->Blit(displayFogTexture, worldDrawPos.x, worldDrawPos.y - 15, { 64, 60 }, &r, 1.0f);
			}

		}
	}

	//LOG("Time: %f", timer.ReadMs() - oldT);
}

//TODO 2: Complete this function: given a position and a flag, create a new entity and return a pointer to it (or nullptr if something has gone wrong)
//Note that the FoWManager needs to know about the entity we are creating, try to find where the FoWManager module stores all the FoWEntities and add it there
//FoWEntity* FoWManager::CreateFoWEntity(iPoint pos, bool providesVisibility)
//{
//	FoWEntity* entity = nullptr;
//
//	entity = new FoWEntity(pos, providesVisibility);
//
//	if (entity != nullptr)
//	{
//		fowEntities.push_back(entity);
//	}
//
//	return entity;
//}


//TODO 5: Complete the following function: it shoud return the tile visibility (true if visible, otherwise false)
//This function will be used to check if we need to draw a certain entity
bool FoWManager::CheckTileVisibility(iPoint mapPos)const
{
	bool ret = false;
	//First check if the entity is inside the map
	//& get the tile fog information,its state, to check if is visible. 
	//Note that the function that you need does both things for you, it is recommended to check and understand what the needed function does

	FoWDataStruct* tileState = GetFoWTileState(mapPos);

	if (tileState != nullptr)
	{
		//Entity will only be visible in visible areas (no fog nor shroud)
		//Think about what happens with the smooth borders, are the considered visble or fogged?
		//Also, do you need to check both the fog and shroud states?
		if (tileState->tileFogBits != fow_ALL)
			ret = true;
	}

	return ret;
}

void FoWManager::MapNeedsUpdate()
{
	if (foWMapNeedsRefresh == false)
		foWMapNeedsRefresh = true;
}

void FoWManager::GetTilesInsideRadius(int fowRadius, fPoint position, iPoint offSet, std::vector<iPoint>& ret)const
{
	int length = (fowRadius * 2) + 1;

	//Adding + (collisionRect.w / 2) maybe bad, think about it
	iPoint startingPos = App->map->WorldToMap(static_cast<int>(position.x) + offSet.x, static_cast<int>(position.y) + offSet.y) - fowRadius;
	iPoint finishingPos = startingPos + length;

	//Creates a vector with all the tiles inside a bounding box delimited by the radius
	for (int i = startingPos.y; i < finishingPos.y; i++)
	{
		for (int j = startingPos.x; j < finishingPos.x; j++)
		{
			ret.push_back({ j,i });
		}
	}
}

//TODO 3: Comprehend and complete this function: (this is the function that does the magic for us)
void FoWManager::ApplyMaskToTiles(int fowRadius, std::vector<iPoint>& tilesAffected)
{
	unsigned short* precMask = &GetMaskFromRadius(fowRadius)[0];
	if (precMask != nullptr)
	{
		for (int i = 0; i < tilesAffected.size(); i++)
		{

			FoWDataStruct* tileValue = GetFoWTileState(tilesAffected[i]);


			if (tileValue != nullptr)
			{
				tileValue->tileShroudBits &= *precMask;
				tileValue->tileFogBits &= *precMask;
			}

			precMask++;
		}
	}
}

unsigned short* FoWManager::GetMaskFromRadius(int radius)
{
	unsigned short* ret = nullptr;

	if (maskMap.count(radius) > 0) //if the key is found
	{
		ret = maskMap.at(radius).mask;
	}


	return ret;
}


void FoWManager::RequestMaskGeneration(int radius)
{

	if (maskMap.count(radius) > 0)
	{
		maskMap.at(radius).numberOfUsers += 1;
	}
	else
	{
		MaskData data;
		data.numberOfUsers = 1;
		data.mask = GenerateCircleJoints(radius, GenerateCircleBorders(radius, GenerateCircle(radius)));
		maskMap.insert(std::pair<uint, MaskData>(radius, data));
		//Fill Corners
		//Fill joints
	}


}


void FoWManager::RequestMaskDeletion(int radius)
{
	if (radius > 0)
	{
		if (maskMap.count(radius) > 0)
		{
			if (maskMap.at(radius).numberOfUsers > 1)
			{
				maskMap.at(radius).numberOfUsers -= 1;
			}
			else
			{
				//delete mask
				RELEASE_ARRAY(maskMap.at(radius).mask);
				maskMap.at(radius).mask = nullptr;
				maskMap.erase(radius);
			}
		}
	}

}


unsigned short* FoWManager::GenerateCircle(int radius)
{
	unsigned short* circle = nullptr;


	int diameter = (radius * 2) + 1;
	iPoint center = { radius,radius };
	circle = new unsigned short[diameter * diameter];

	for (int y = 0; y < diameter; y++)
	{
		for (int x = 0; x < diameter; x++)
		{
			if (InsideCircle(center, { x,y }, radius) == true)
			{
				circle[(y * diameter) + x] = fow_NON;
			}
			else
			{
				circle[(y * diameter) + x] = fow_ALL;
			}
		}
	}

	return circle;
}

unsigned short* FoWManager::GenerateCircleBorders(int radius, unsigned short* mask)
{
	int diameter = (radius * 2) + 1;

	for (int y = 0; y < diameter; y++)
	{


		for (int x = 0; x < diameter; x++)
		{


			if (mask[(y * diameter) + x] == fow_NON)
			{
				//do tile check and change
				unsigned short aux = CheckCornersFromNeighbours({ x,y }, diameter, mask);
				switch (aux)
				{
				case fow_neighbour_W:
					mask[(y * diameter) + x] = fow_WWW;
					break;
				case fow_neighbour_E:
					mask[(y * diameter) + x] = fow_EEE;
					break;
				case fow_neighbour_N:
					mask[(y * diameter) + x] = fow_NNN;
					break;
				case fow_neighbour_S:
					mask[(y * diameter) + x] = fow_SSS;
					break;
				case fow_neighbour_CNE:
					mask[(y * diameter) + x] = fow_CNE;
					break;
				case fow_neighbour_CNW:
					mask[(y * diameter) + x] = fow_CNW;
					break;
				case fow_neighbour_CSE:
					mask[(y * diameter) + x] = fow_CSE;
					break;
				case fow_neighbour_CSW:
					mask[(y * diameter) + x] = fow_CSW;
					break;
				case fow_neighbour_HE:
					mask[(y * diameter) + x] = fow_ALL;
					mask[(y * diameter) + x + 1] = fow_WWW;
					break;
				case fow_neighbour_HW:
					mask[(y * diameter) + x] = fow_ALL;
					mask[(y * diameter) + x - 1] = fow_EEE;
					break;
				case fow_neighbour_HN:
					mask[(y * diameter) + x] = fow_ALL;
					mask[((y - 1) * diameter) + x] = fow_SSS;
					break;
				case fow_neighbour_HS:
					mask[(y * diameter) + x] = fow_ALL;
					mask[((y + 1) * diameter) + x + 1] = fow_NNN;
					break;
				}
			}


		}


	}

	return mask;
}

unsigned short* FoWManager::GenerateCircleJoints(int radius, unsigned short* mask)
{
	int diameter = (radius * 2) + 1;

	for (int y = 0; y < diameter; y++)
	{


		for (int x = 0; x < diameter; x++)
		{


			if (mask[(y * diameter) + x] == fow_NON)
			{
				//do tile check and change
				unsigned short aux = CheckJointsFromNeighbours({ x,y }, diameter, mask);

				switch (aux)
				{
				case fow_neighbour_CNE:
					mask[(y * diameter) + x] = fow_JNE;
					break;
				case fow_neighbour_CNW:
					mask[(y * diameter) + x] = fow_JNW;
					break;
				case fow_neighbour_CSE:
					mask[(y * diameter) + x] = fow_JSE;
					break;
				case fow_neighbour_CSW:
					mask[(y * diameter) + x] = fow_JSW;
					break;
				}




			}


		}


	}

	return mask;
}


bool FoWManager::InsideCircle(iPoint center, iPoint tile, float radius)
{
	float distance_squared = center.DistanceNoSqrt(tile);

	return distance_squared < (radius + 0.5f) * (radius + 0.5f);
}

unsigned short FoWManager::CheckCornersFromNeighbours(iPoint pos, int diameter, unsigned short* mask)
{
	unsigned short ret = 0;

	//check West tile
	if (pos.x == 0)
	{
		ret += fow_neighbour_W;
	}
	else
	{
		if (mask[(pos.y * diameter) + pos.x - 1] == fow_ALL)
		{
			ret += fow_neighbour_W;
		}
	}

	//check East tile
	if (pos.x == diameter - 1)
	{
		ret += fow_neighbour_E;
	}
	else
	{
		if (mask[(pos.y * diameter) + pos.x + 1] == fow_ALL)
		{
			ret += fow_neighbour_E;
		}
	}

	//check North tile
	if (pos.y == 0)
	{
		ret += fow_neighbour_N;
	}
	else
	{
		if (mask[((pos.y - 1) * diameter) + pos.x] == fow_ALL)
		{
			ret += fow_neighbour_N;
		}
	}

	//check South tile
	if (pos.y == diameter - 1)
	{
		ret += fow_neighbour_S;
	}
	else
	{
		if (mask[((pos.y + 1) * diameter) + pos.x] == fow_ALL)
		{
			ret += fow_neighbour_S;
		}
	}

	return ret;
}

unsigned short FoWManager::CheckJointsFromNeighbours(iPoint pos, int diameter, unsigned short* mask)
{
	unsigned short ret = 0;
	int leftTileId = (pos.y * diameter) + pos.x - 1;
	int rightTileId = (pos.y * diameter) + pos.x + 1;
	int upperTileId = ((pos.y - 1) * diameter) + pos.x;
	int bottomTileId = ((pos.y + 1) * diameter) + pos.x;





	if (mask[leftTileId] == fow_CNW || mask[leftTileId] == fow_CNE || mask[leftTileId] == fow_CSE || mask[leftTileId] == fow_CSW ||
		mask[leftTileId] == fow_WWW || mask[leftTileId] == fow_SSS || mask[leftTileId] == fow_NNN || mask[leftTileId] == fow_EEE)
	{
		ret += fow_neighbour_W;

	}

	//check East tile

	if (mask[rightTileId] == fow_CNW || mask[rightTileId] == fow_CNE || mask[rightTileId] == fow_CSE || mask[rightTileId] == fow_CSW ||
		mask[rightTileId] == fow_WWW || mask[rightTileId] == fow_SSS || mask[rightTileId] == fow_NNN || mask[rightTileId] == fow_EEE)
	{
		ret += fow_neighbour_E;
	}


	//check North tile

	if (mask[upperTileId] == fow_CNW || mask[upperTileId] == fow_CNE || mask[upperTileId] == fow_CSE || mask[upperTileId] == fow_CSW ||
		mask[upperTileId] == fow_WWW || mask[upperTileId] == fow_SSS || mask[upperTileId] == fow_NNN || mask[upperTileId] == fow_EEE)
	{
		ret += fow_neighbour_N;
	}


	//check South tile

	if (mask[bottomTileId] == fow_CNW || mask[bottomTileId] == fow_CNE || mask[bottomTileId] == fow_CSE || mask[bottomTileId] == fow_CSW ||
		mask[bottomTileId] == fow_WWW || mask[bottomTileId] == fow_SSS || mask[bottomTileId] == fow_NNN || mask[bottomTileId] == fow_EEE)
	{
		ret += fow_neighbour_S;
	}


	return ret;
}
