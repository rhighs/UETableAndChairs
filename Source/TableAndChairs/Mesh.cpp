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
	CubeMesh(mesh, size, FVector::ZeroVector);
}

void CubeMesh(Mesh& mesh, const FVector& size, const FVector& vertexOffset)
{
	int32 nVertices = 24;
	int32 nTriangles = 12;
	mesh.Allocate(nVertices, nTriangles);

	// Calculate a half offset so we get correct center of object
	const float OffsetX = size.X / 2.0f;
	const float OffsetY = size.Y / 2.0f;
	const float OffsetZ = size.Z / 2.0f;

	// Define the 8 corners of the cube
	const FVector p0 = FVector(OffsetX,  OffsetY, -OffsetZ) + vertexOffset;
	const FVector p1 = FVector(OffsetX, -OffsetY, -OffsetZ) + vertexOffset;
	const FVector p2 = FVector(OffsetX, -OffsetY,  OffsetZ) + vertexOffset;
	const FVector p3 = FVector(OffsetX,  OffsetY,  OffsetZ) + vertexOffset;
	const FVector p4 = FVector(-OffsetX, OffsetY, -OffsetZ) + vertexOffset;
	const FVector p5 = FVector(-OffsetX, -OffsetY, -OffsetZ) + vertexOffset;
	const FVector p6 = FVector(-OffsetX, -OffsetY, OffsetZ) + vertexOffset;
	const FVector p7 = FVector(-OffsetX, OffsetY, OffsetZ) + vertexOffset;

	// Add each face given their vertices, normals and tangents
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
	int32 iota = mesh.Vertices.Num();
	int32 index1 = iota++;
	int32 index2 = iota++;
	int32 index3 = iota++;
	int32 index4 = iota++;

	mesh.Vertices.Add(bottomLeft);
	mesh.Vertices.Add(bottomRight);
	mesh.Vertices.Add(topRight);
	mesh.Vertices.Add(topLeft);

	mesh.UVs.Add(FVector2D(0.0f, 1.0f));
	mesh.UVs.Add(FVector2D(1.0f, 1.0f));
	mesh.UVs.Add(FVector2D(1.0f, 0.0f));
	mesh.UVs.Add(FVector2D(0.0f, 0.0f));

	mesh.Triangles.Add(index1);
	mesh.Triangles.Add(index2);
	mesh.Triangles.Add(index3);
	mesh.Triangles.Add(index1);
	mesh.Triangles.Add(index3);
	mesh.Triangles.Add(index4);

	mesh.Normals.Append({ normal, normal, normal, normal });
	mesh.Tangents.Append({ tangent, tangent, tangent, tangent });
} 

void BuildCylinder(Mesh& mesh, int32 sides, FVector size, FVector location, bool withTopCap, bool withBottomCap)
{
	mesh.Empty();

	int32 vertexCount = 6*sides+2;
	int32 trianglesCount = 2 * sides;
	mesh.Allocate(vertexCount, trianglesCount);

	auto& uv = mesh.UVs;
	auto& normals = mesh.Normals;
	auto& triangles = mesh.Triangles;
	auto& vertices = mesh.Vertices;
	auto& tangents = mesh.Tangents;

	float angleStep = 2 * PI / sides;

	int32 id = 0;
	if (withBottomCap)
	{
		for (int t = 0; t <= sides; ++t)
		{
			if (t == 0)
			{
				vertices.Add(FVector(location.X, location.Y, location.Z));
				uv.Add(FVector2D(0.5, 0.5));
				normals.Add(FVector::DownVector);
				tangents.Add(FProcMeshTangent(FVector::LeftVector, false));

				++id;
				continue;
			}

			float x = FMath::Cos(PI / 4+angleStep * t);
			float y = FMath::Sin(PI / 4 + angleStep * t);

			FVector v = FVector(location.X+x * size.X, location.Y+y * size.Y, location.Z);

			vertices.Add(v);

			int32 id2 = (t % sides) + 1;

			triangles.Add(0);
			triangles.Add(id);
			triangles.Add(id2);

			normals.Add(FVector::DownVector);
			tangents.Add(FProcMeshTangent(FVector::LeftVector, false));
			uv.Add(FVector2D((x + 1) / 2,
							 (+y + 1) / 2));

			++id;
		}
	}

	// Mesh strip
	for (int t = 0; t < sides; ++t)
	{
		float x1 = FMath::Cos(PI / 4 + angleStep * t);
		float y1 = FMath::Sin(PI / 4 + angleStep * t);

		float x2 = FMath::Cos(PI / 4 + angleStep * (t + 1));
		float y2 = FMath::Sin(PI / 4 + angleStep * (t + 1));

		FVector bottomRight(location.X+x1*size.X, location.Y + y1*size.Y, location.Z);
		FVector topRight(location.X + x1 * size.X, location.Y + y1 * size.Y, location.Z+size.Z);
		FVector bottomLeft(location.X + x2 * size.X, location.Y + y2 * size.Y, location.Z);
		FVector topLeft(location.X + x2 * size.X, location.Y + y2 * size.Y, location.Z+size.Z);
		vertices.Add(bottomRight);	// id
		vertices.Add(topRight);		// id+1
		vertices.Add(bottomLeft);	// id+2
		vertices.Add(topLeft);		// id+3

		// bottom-right triangle
		triangles.Add(id);
		triangles.Add(id + 1);
		triangles.Add(id + 2);
		// top-left triangle
		triangles.Add(id + 2);
		triangles.Add(id + 1);
		triangles.Add(id + 3);

		uv.Add(FVector2D(1 - (1.f / sides) * t, 0)); // bottomRight
		uv.Add(FVector2D(1 - (1.f / sides) * t, 1)); // topRight
		uv.Add(FVector2D(1 - (1.f / sides) * (t + 1), 0)); // bottomLeft
		uv.Add(FVector2D(1 - (1.f / sides) * (t + 1), 1)); // topLeft

		FVector faceNormal = (bottomRight + bottomLeft).GetSafeNormal();
		normals.Add(faceNormal);
		normals.Add(faceNormal);
		normals.Add(faceNormal);
		normals.Add(faceNormal);

		FVector faceTangent = (bottomLeft - bottomRight).GetSafeNormal();
		tangents.Add(FProcMeshTangent(faceTangent, false));
		tangents.Add(FProcMeshTangent(faceTangent, false));
		tangents.Add(FProcMeshTangent(faceTangent, false));
		tangents.Add(FProcMeshTangent(faceTangent, false));

		id += 4;
	}

	// Building top base
	int32 centerID = id;
	if (withTopCap)
	{
		for (int32 t = 0; t <= sides; ++t)
		{
			if (t == 0)
			{
				vertices.Add(FVector(location.X, location.Y, location.Z+size.Z));
				uv.Add(FVector2D(0.5, 0.5));
				normals.Add(FVector::UpVector);
				tangents.Add(FProcMeshTangent(FVector::ForwardVector, false));

				++id;
				continue;
			}

			float x = FMath::Cos(PI / 4 + angleStep * (t-1));
			float y = FMath::Sin(PI / 4 + angleStep * (t-1));

			FVector v = FVector(location.X+x * size.X, location.Y+ y * size.Y, location.Z+size.Z);

			vertices.Add(v);

			//int32 id2 = centerID + (t % sides) +1;
			int32 id2 = id + 1;
			if (id2 > centerID + sides)
				id2 = centerID + 1;

			triangles.Add(centerID);
			triangles.Add(id2);
			triangles.Add(id);

			normals.Add(FVector::UpVector);
			tangents.Add(FProcMeshTangent(FVector::ForwardVector, false));
			uv.Add(FVector2D((x + 1) / 2,
				(y + 1) / 2));

			++id;
		}
	}

}
