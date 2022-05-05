#include "Mesh.h"

void Mesh::Allocate(int32 nVertices, int32 nTriangles)
{
	Vertices.Reserve(nVertices);
	Triangles.Reserve(nTriangles * 3);
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

void CubeMesh(Mesh& mesh, const FVector& size)
{
	int32 nVertices = 24;
	int32 nTriangles = 12;
	mesh.Allocate(nVertices, nTriangles);

	// Calculate a half offset so we get correct center of object
	const float OffsetX = size.X / 2.0f;
	const float OffsetY = size.Y / 2.0f;
	const float OffsetZ = size.Z / 2.0f;

	// Define the 8 corners of the cube
	const FVector p0 = FVector(OffsetX,  OffsetY, -OffsetZ);
	const FVector p1 = FVector(OffsetX, -OffsetY, -OffsetZ);
	const FVector p2 = FVector(OffsetX, -OffsetY,  OffsetZ);
	const FVector p3 = FVector(OffsetX,  OffsetY,  OffsetZ);
	const FVector p4 = FVector(-OffsetX, OffsetY, -OffsetZ);
	const FVector p5 = FVector(-OffsetX, -OffsetY, -OffsetZ);
	const FVector p6 = FVector(-OffsetX, -OffsetY, OffsetZ);
	const FVector p7 = FVector(-OffsetX, OffsetY, OffsetZ);

	AddRectangleTo(mesh, p0, p1, p2, p3, FVector::RightVector, FProcMeshTangent(FVector::ForwardVector, false));
	AddRectangleTo(mesh, p5, p4, p7, p6, FVector::LeftVector, FProcMeshTangent(FVector::ForwardVector, false));
	AddRectangleTo(mesh, p1, p5, p6, p2, FVector::ForwardVector, FProcMeshTangent(FVector::RightVector, false));
	AddRectangleTo(mesh, p4, p0, p3, p7, FVector::BackwardVector, FProcMeshTangent(FVector::RightVector, false));
	AddRectangleTo(mesh, p6, p7, p3, p2, FVector::UpVector, FProcMeshTangent(FVector::RightVector, false));
	AddRectangleTo(mesh, p1, p0, p4, p5, FVector::DownVector, FProcMeshTangent(FVector::RightVector, false));
}

void AddRectangleTo(Mesh& mesh,
	const FVector& bottomLeft, const FVector& bottomRight,
	const FVector& topRight, const FVector& topLeft,
	const FVector& normal, const FProcMeshTangent& tangent)
{
	const int32 index1 = mesh.Vertices.Num();
	const int32 index2 = mesh.Vertices.Num() + 1;
	const int32 index3 = mesh.Vertices.Num() + 2;
	const int32 index4 = mesh.Vertices.Num() + 3;

	mesh.Vertices[index1] = bottomLeft;
	mesh.Vertices[index2] = bottomRight;
	mesh.Vertices[index3] = topRight;
	mesh.Vertices[index4] = topLeft;

	mesh.UVs[index1] = FVector2D(0.0f, 1.0f);
	mesh.UVs[index2] = FVector2D(1.0f, 1.0f);
	mesh.UVs[index3] = FVector2D(1.0f, 0.0f);
	mesh.UVs[index4] = FVector2D(0.0f, 0.0f);

	mesh.Triangles[mesh.Triangles.Num()] = index1;
	mesh.Triangles[mesh.Triangles.Num() + 1] = index2;
	mesh.Triangles[mesh.Triangles.Num() + 2] = index3;
	mesh.Triangles[mesh.Triangles.Num() + 3] = index4;

	mesh.Normals[index1] = mesh.Normals[index2] = mesh.Normals[index3] = mesh.Normals[index4] = normal;
	mesh.Tangents[index1] = mesh.Tangents[index2] = mesh.Tangents[index3] = mesh.Tangents[index4] = tangent;
} 

