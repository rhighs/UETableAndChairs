#pragma once

#include "CoreMinimal.h"

struct Mesh
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;
	TArray<FVector> Normals;
	TArray<FVector> Tangents;

	Mesh() = default;

	void Allocate(int32 nVertices, int32 nTriangles);
	void Empty();
};

