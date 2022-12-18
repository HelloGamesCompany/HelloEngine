#ifndef __XMLNODE_H__
#define __XMLNODE_H__

#include "Globals.h"
#include "pugixml.hpp"
#include <queue>

class XMLNode
{
public:
	pugi::xml_node node;

	/// <summary>
	/// Search through all children nodes from this current node to find a child with a matching name.
	/// </summary>
	/// <param name="name">Child name</param>
	/// <returns></returns>
	XMLNode FindChildBreadth(std::string name);

	XMLNode FindChildDepth(std::string name);

	void Save(const std::string& extension = ".xml");

	bool operator == (const XMLNode& n) const
	{
		return (n.node == this->node);
	}

	bool operator != (const XMLNode& n) const
	{
		return (n.node != this->node);
	}

private:

	XMLNode FindChildBreadthRecursive(std::queue<pugi::xml_node> q, std::string name);

	XMLNode FindChildDepthRecursive(pugi::xml_node node, std::string name, bool& condition);

private:
	uint xmlFile = 0;

	friend class ModuleXML;
};

#endif // !__XMLNODE_H__