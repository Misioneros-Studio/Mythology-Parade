#include "j1App.h"
#include "AssetsManager.h"
#include "p2Log.h"
#include "p2Defs.h"


AssetsManager::AssetsManager() : j1Module()
{
	name = ("assetsManager");

	PHYSFS_init(nullptr);

	PHYSFS_mount(".", nullptr, 1);
}


AssetsManager::~AssetsManager()
{
	PHYSFS_deinit();
}

bool AssetsManager::Awake(pugi::xml_node& config)
{

	if (PHYSFS_isInit())
		LOG("Asset Manager is succefully loaded");
	else {
		LOG("Failed loading Asset Manager");
		return false;
	}
	PHYSFS_mount("Assets.zip", nullptr, 1);

	return true;
}


bool AssetsManager::CleanUp()
{
	return false;
}

SDL_RWops* AssetsManager::Load(const char* path) const
{
	char* buffer;
	uint bytes = Load(path, &buffer);

	SDL_RWops* ret = SDL_RWFromConstMem(buffer, bytes);

	return ret;

}


uint AssetsManager::Load(const char* path, char** buffer) const
{
	uint ret = 0;

	PHYSFS_file* file = PHYSFS_openRead(path);
	if (!PHYSFS_eof(file)) {
		uint length = PHYSFS_fileLength(file);

		*buffer = new char[length];

		uint bytes = PHYSFS_readBytes(file, *buffer, length);
		if (bytes != length)
		{
			LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());
			RELEASE_ARRAY(buffer);
		}
		else
			ret = bytes;
	}
	else
		LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());

	PHYSFS_close(file);

	return ret;
}
