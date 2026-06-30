#include <string>
#include <filesystem>

#include <glm/glm.hpp>

enum class JointTypes
{
	Revolute,
	Prismatic,
	Helical,
	Cylindrical,
	Universal,
	Spherical
};

struct Joint
{
	std::string name;
	std::filesystem::path path;
	JointTypes type;
	glm::vec3 axis;
};

class RevoluteJoint : protected Joint
{

};

class PrismaticJoint : protected Joint
{

};

class HelicalJoint : protected Joint
{

};

class CylindricalJoint : protected Joint
{

};

class UniversalJoint : protected Joint
{

};

class SphericalJoint : protected Joint
{

};
