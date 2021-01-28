#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "Material.h"

/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};

	enum GEOMETRY_TYPE {
		GEO_TRIANGLE_1,
		GEO_TRIANGLE_2,
		GEO_TRIANGLE_3,
		NUM_GEOMETRY,
	};

	Mesh(const std::string &meshName);
	~Mesh();
	void Render();
	void Render(unsigned offset, unsigned count);
	static void SetMaterialLoc(unsigned kA, unsigned kD, unsigned kS, unsigned nS);

	
	const std::string	  name;
	static unsigned		  locationKa;
	static unsigned		  locationKd;
	static unsigned		  locationKs;
	static unsigned		  locationNs;
	std::vector<Material> materials;
	Material			  material;

	DRAW_MODE			  mode;
	
	unsigned			  vertexBuffer;
	unsigned			  indexBuffer;
	unsigned			  indexSize;
	unsigned			  textureID;
};

#endif	