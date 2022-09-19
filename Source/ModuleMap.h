#pragma once

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "External/PugiXml/src/pugixml.hpp"

#include "External/SDL/include/SDL.h"

struct MapObject
{
	int id = 0;
	int dimensions[3] = { 1,1,1 };
	iPoint position = { 0,0 };
};

// L03: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	std::string	name = "";
	int	firstgid = 0;
	int margin = 0;
	int	spacing = 0;
	int	tileWidth = 0;
	int	tileHeight = 0;

	//SDL_Texture* texture = nullptr;
	int	texWidth = 0;
	int	texHeight = 0;
	int	tilecount = 0;
	int	columns = 0;

	// L04: DONE 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

// L03: DONE 1: We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// L06: TODO 5: Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		//...
		std::string name = "";
		int value = 0;
	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	// L06: TODO 7: Method to ask for the value of a custom property
	int GetProperty(const char* name, int default_value = 0) const;

	List<Property*> list;
};

// L04: DONE 1: Create a struct for the map layer
struct MapLayer
{
	std::string	name = "";
	int width = 0;
	int height = 0;
	uint* data = nullptr;

	// L06: DONE 1: Support custom properties
	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: DONE 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

// L03: DONE 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width = 0;
	int	height = 0;
	int	tileWidth = 0;
	int	tileHeight = 0;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;

	// L04: DONE 2: Add a list/array of layers to the map
	List<MapLayer*> layers;
};

class ModuleMap : public Module
{
public:

	ModuleMap(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleMap();

	// Called before quitting
	bool CleanUp() override;

	// Load new map
	bool Load(const char* path);

	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	// L05: DONE 2: Add orthographic world to map coordinates
	iPoint WorldToMap(int x, int y) const;

	void LoadLayerMeta();

private:

	// L03: Methods to load all required map data
	bool LoadMap(pugi::xml_node mapFile);
	bool LoadTileSets(pugi::xml_node mapFile);
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);

	// L04
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	// L06: TODO 6: Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	// L06: TODO 3: Pick the right Tileset based on a tile id
	TileSet* GetTilesetFromTileId(int id) const;

public:

	// L03: DONE 1: Add your struct for map info
	MapData mapData;

	List<MapObject> mapObjects;

private:

	std::string currentMap = "null";
	std::string folder = "Assets/Map/";
	bool mapLoaded;
};


