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

	void Allocate(int32 nVertices, int32 nTriangles);
	void Empty();
};

void CubeMesh(Mesh& mesh, const FVector& size);

void AddRectangleTo(Mesh& mesh,
	const FVector& bottomLeft, const FVector& bottomRight,
	const FVector& topRight, const FVector& topLeft,
	const FVector& normal, const FProcMeshTangent& tangent);


