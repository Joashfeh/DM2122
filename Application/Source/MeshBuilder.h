#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float size = 1.f);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float size = 1.f);
	static Mesh* GenerateCircle(const std::string& meshName, Color color, unsigned numSlice, float size = 1.f);
	static Mesh* GenerateSphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float size = 1.f);
	static Mesh* GenerateHemisphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float size = 1.f);
	static Mesh* GenerateRing(const std::string& meshName, Color color, unsigned numSlice, float innerRad = 1.f, float outerRad = 0.8f);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height);
	static Mesh* GenerateCone(const std::string& meshName, Color color, unsigned numSlice, float rad = 1.f, float height = 1.f);
	static Mesh* GenerateTorus(const std::string& meshName, Color color, unsigned numstack, unsigned numslice, float outerR = 1.f, float innerR = 0.5f);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);
	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);

};

#endif