#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

struct Mesh
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;

	Mesh() = default;

	/** Allocate the underlying memory of the TArrays
	* \param the number of vertices
	* \param the number of triangles (not to think as "the number of ids")
	*/
	void Allocate(int32 nVertices, int32 nTriangles);

	/** Increment each vertex in `Vertices` by a given offset
	* \param the vector to consider as offset
	*/
	void Translate(const FVector& offset);

	/** Rotate each vertex in `Vertices` by a given triplet of angles (in deg)
	* \param the vector to consider as triplet of angles
	*/
	void Rotate(const FVector& rotationDegrees);

	/** Deallocate all buffers
	*/
	void Empty();
};

/** Generate a cubic mesh given a size with origin (0, 0, 0)
* \param a mutable reference to a Mesh where the genereted cube will be added
* \param the cube size
*/
void CubeMesh(Mesh& mesh, const FVector& size);

/** Generate a cubic mesh given a size with origin at vertexOffset
* \param a mutable reference to a Mesh where the genereted cube will be added
* \param the cube size
* \param the origin offset
*/
void CubeMesh(Mesh& mesh, const FVector& size, const FVector& vertexOffset);

/** Generate a spherical mesh given a radius, number of parallels and number of meridians
* \param a mutable reference to a Mesh where the generated sphere will be added
* \param the sphere radius
* \param the number of parallels
* \param the number of meridians
*/
void SphereMesh(Mesh& mesh, float radius, int32 parallels, int32 meridians);

/** Generate a spherical mesh given a radius, number of parallels and number of meridians
* \param a mutable reference to a Mesh where the generated sphere will be added
* \param the sphere radius
* \param the number of parallels
* \param the number of meridians
* \param the origin offset
*/
void SphereMesh(Mesh& mesh, float radius, int32 parallels, int32 meridians, const FVector& sphereOrigin);

/** Add a 2D rectanle to a given mesh
* \param a mutable reference to a Mesh where the rectangel will be added
* \param bottom left vertex position
* \param bottom right vertex position
* \param top right vertex position
* \param top left vertex position
* \param the surface normal for the rectangle
* \param the surface tangent for the rectangle
*/
void AddRectangleTo(Mesh& mesh,
	const FVector& bottomLeft, const FVector& bottomRight,
	const FVector& topRight, const FVector& topLeft,
	const FVector& normal, const FProcMeshTangent& tangent);

/** Generate a cylindrical mesh
* TODO: finish docs
*/
void BuildCylinder(Mesh& mesh, int32 sides, FVector size, FVector location, bool withTopCap, bool withBottomCap);

