#include "Mesh.h"

void Mesh::Allocate(int32 nVertices, int32 nTriangles)
{
	Vertices.Reserve(nVertices);
	Triangles.Reserve(nTriangles);
	UVs.Reserve(nVertices);
	Normals.Reserve(nVertices);
	Tangents.Reserve(nVertices);
}

void Mesh::Empty()
{
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
	Normals.Empty();
	Tangents.Empty();
}

