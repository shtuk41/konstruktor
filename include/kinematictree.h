#ifndef _KINEMATICTREE_H
#define _KINEMATICTREE_H

#include <vector>

#include <joints.h>

struct Node
{
	std::vector<int> children;
	int parent;
	Joint parentJoint;
};

class KinematicTree
{
	std::vector<Node> nodes;
	std::vector<int> rootIndices;
};

#endif