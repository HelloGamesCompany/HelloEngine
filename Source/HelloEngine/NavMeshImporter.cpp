#include "Headers.h"
#include "NavMeshImporter.h"
#include "ModuleFiles.h"
#include "ModuleNavMesh.h"
#include "json.hpp"

using json = nlohmann::json;

bool NavMeshImporter::SaveNavMesh(const char* assets_path, const dtNavMesh* const navMesh, const BuildSettings* const buildSettings)
{
	if (!navMesh)
		return false;

	json data;

	// store setting
	data["buildSetting"] =
	{
		{"cellSize",buildSettings->cellSize},
		{"cellHeight",buildSettings->cellHeight},
		{"agentHeight",buildSettings->agentHeight},
		{"agentRadius",buildSettings->agentRadius},
		{"agentMaxClimb",buildSettings->agentMaxClimb},
		{"agentMaxSlope",buildSettings->agentMaxSlope},
		{"regionMinSize",buildSettings->regionMinSize},
		{"regionMergeSize",buildSettings->regionMergeSize},
		{"edgeMaxLen",buildSettings->edgeMaxLen} ,
		{"edgeMaxError",buildSettings->edgeMaxError},
		{"vertsPerPoly",buildSettings->vertsPerPoly},
		{"detailSampleDist",buildSettings->detailSampleDist},
		{"detailSampleMaxError",buildSettings->detailSampleMaxError},
		{"partitionType",buildSettings->partitionType},
		{"navMeshBMin",{buildSettings->navMeshBMin[0],buildSettings->navMeshBMin[1],buildSettings->navMeshBMin[2]}},
		{"navMeshBMax",{buildSettings->navMeshBMax[0],buildSettings->navMeshBMax[1],buildSettings->navMeshBMax[2]}},
		{"tileSize",buildSettings->tileSize}
	};

	// Store header.
	NavMeshSetHeader header;
	header.numTiles = 0;

	header.params =  *navMesh->getParams();

	for (int i = 0; i < navMesh->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = navMesh->getTile(i);

		if (!tile || !tile->header || !tile->dataSize)
			continue;

		header.numTiles++;
	}

	data["navMeshSetHeader"] =
	{
		{"numTiles", header.numTiles},
		{"params",
			{
				{"orig",{header.params.orig[0],header.params.orig[1],header.params.orig[2]}},
				{"tileWidth",header.params.tileWidth},
				{"tileHeight",header.params.tileHeight},
				{"maxTiles",header.params.maxTiles},
				{"maxPolys",header.params.maxPolys}
			}
		}
	};

	// Store tiles
	data["tiles"] = nlohmann::json::array();

	for (int i = 0; i < navMesh->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = navMesh->getTile(i);

		if (!tile || !tile->header || !tile->dataSize)
			continue;

		std::vector<unsigned char> bytes(tile->data, tile->data + tile->dataSize);

		json new_object =
		{
			{"tileRef", navMesh->getTileRef(tile)},
			{"dataSize", tile->dataSize},
			{"data", bytes}
		};

		data["tiles"].push_back(new_object);
	}

	std::string buffer = data.dump(2);

	ModuleFiles::S_Save(assets_path, &buffer[0], buffer.size(), false);

	return true;
}

bool NavMeshImporter::LoadNavMesh(const char* assets_path, dtNavMesh* navMesh, BuildSettings* buildSettings)
{
	char* buffer = nullptr;

	if (ModuleFiles::S_Load(assets_path, &buffer) == 0)
		return false;

	json data = json::parse(buffer);

	RELEASE(buffer);

	// Load build setting
	{
		buildSettings->cellSize = data["buildSetting"]["cellSize"];
		buildSettings->cellHeight = data["buildSetting"]["cellHeight"];
		buildSettings->agentRadius = data["buildSetting"]["agentRadius"];
		buildSettings->agentMaxClimb = data["buildSetting"]["agentMaxClimb"];
		buildSettings->agentMaxSlope = data["buildSetting"]["agentMaxSlope"];
		buildSettings->regionMinSize = data["buildSetting"]["regionMinSize"];
		buildSettings->regionMergeSize = data["buildSetting"]["regionMergeSize"];
		buildSettings->edgeMaxLen = data["buildSetting"]["edgeMaxLen"];
		buildSettings->edgeMaxError = data["buildSetting"]["edgeMaxError"];
		buildSettings->vertsPerPoly = data["buildSetting"]["vertsPerPoly"];
		buildSettings->detailSampleDist = data["buildSetting"]["detailSampleDist"];
		buildSettings->detailSampleMaxError = data["buildSetting"]["detailSampleMaxError"];
		buildSettings->partitionType = data["buildSetting"]["partitionType"];
		buildSettings->navMeshBMin[0] = data["buildSetting"]["navMeshBMin"][0];
		buildSettings->navMeshBMin[1] = data["buildSetting"]["navMeshBMin"][1];
		buildSettings->navMeshBMin[2] = data["buildSetting"]["navMeshBMin"][2];
		buildSettings->navMeshBMax[0] = data["buildSetting"]["navMeshBMax"][0];
		buildSettings->navMeshBMax[1] = data["buildSetting"]["navMeshBMax"][1];
		buildSettings->navMeshBMax[2] = data["buildSetting"]["navMeshBMax"][2];
		buildSettings->tileSize = data["buildSetting"]["tileSize"];
	}

	// Load build header
	navMesh = dtAllocNavMesh();

	if (!navMesh)
		return false;

	NavMeshSetHeader header;
	header.numTiles = data["navMeshSetHeader"]["numTiles"];
	header.params.orig[0] = data["navMeshSetHeader"]["params"]["orig"][0];
	header.params.orig[1] = data["navMeshSetHeader"]["params"]["orig"][1];
	header.params.orig[2] = data["navMeshSetHeader"]["params"]["orig"][2];
	header.params.tileWidth = data["navMeshSetHeader"]["params"]["tileWidth"];
	header.params.tileHeight = data["navMeshSetHeader"]["params"]["tileHeight"];
	header.params.maxTiles = data["navMeshSetHeader"]["params"]["maxTiles"];
	header.params.maxPolys = data["navMeshSetHeader"]["params"]["maxPolys"];

	dtStatus status = navMesh->init(&header.params);

	if (dtStatusFailed(status))
		return false;

	// Load tiles
	for (int i = 0; i < header.numTiles; ++i)
	{
		dtTileRef tr = data["tiles"][i]["tileRef"];
		int tsize = data["tiles"][i]["dataSize"];
		std::vector<unsigned char>bytes = data["tiles"][i]["data"];

		navMesh->addTile(bytes.data(), tsize, DT_TILE_FREE_DATA, tr, 0);
	}

	return true;
}
