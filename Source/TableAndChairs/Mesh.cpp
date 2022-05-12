#include "Mesh.h"
#include "ProceduralMeshComponent.h"

static TArray<int32> SquareTrianglesIds(int32 topLeft, int32 topRight, int32 bottomLeft, int32 bottomRight)
{
    return {
        topLeft,
        topRight,
        bottomRight,
        bottomRight,
        bottomLeft,
        topLeft
    };
}

static TArray<FVector> CircularNGonPoints(int32 nPoints, float radius, const FVector& center, float theta)
{
	TArray<FVector> points;
	points.AddUninitialized(nPoints + 1);

	float dt = (PI * 2) / nPoints;

	for (auto& point : points)
	{
		point.X = center.X + radius * FMath::Cos(theta);
		point.Y = center.Y + radius * FMath::Sin(theta);
		point.Z = center.Z;

		theta += dt;
	}

	return points;
}

static TArray<int32> StripFromNGonCouple(const TArrayView<int32>& topNGonIds, const TArrayView<int32>& bottomNGonIds)
{
	if (topNGonIds.Num() != bottomNGonIds.Num()) {
		return {};
	}

	TArray<int32> ids;
	// Each iteration apppends 6 ids;
	ids.Reserve(topNGonIds.Num() * 6);

	auto it1 = topNGonIds.begin();
	auto it2 = bottomNGonIds.begin();

	for (; it1 < topNGonIds.end() - 1;) {
		int32 topLeft = *it1;
		int32 topRight = *(it1 + 1);
		int32 bottomLeft = *it2;
		int32 bottomRight = *(it2 + 1);

		ids.Append(SquareTrianglesIds(topLeft, topRight, bottomLeft, bottomRight));

		it1++; it2++;
	}

	return ids;
}

void Mesh::Allocate(int32 nVertices, int32 nTriangles)
{
	int32 nTriangleIds = nTriangles * 3;

	Vertices.Reserve(nVertices);
	UVs.Reserve(nVertices);
	Normals.Reserve(nVertices);
	Tangents.Reserve(nVertices);
	Triangles.Reserve(nTriangleIds);
}

void Mesh::Empty()
{
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
	Normals.Empty();
	Tangents.Empty();
}

void Mesh::Translate(const FVector& offset)
{
	for (auto& vertex : Vertices)
		vertex += offset;
}

void Mesh::Rotate(const FVector& rotationDegrees)
{
	for (auto& vertex : Vertices)
	{
		const FRotator rotator(rotationDegrees.X, rotationDegrees.Y, rotationDegrees.Z);
		vertex = rotator.RotateVector(vertex);
	}
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
	//mesh.Empty();

	int32 vertexCount = 6*sides+2;
	int32 trianglesCount = 2 * sides;
	mesh.Allocate(vertexCount, trianglesCount);

	auto& uv = mesh.UVs;
	auto& normals = mesh.Normals;
	auto& triangles = mesh.Triangles;
	auto& vertices = mesh.Vertices;
	auto& tangents = mesh.Tangents;

	float angleStep = 2 * PI / sides;
	//FVector halfSize = size / 2;
	FVector halfSize = FVector(size.X / 2, size.Y / 2, size.Z);

	int32 indexOffset = vertices.Num();
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

			FVector v = FVector(location.X+x * halfSize.X, location.Y+y * halfSize.Y, location.Z);

			vertices.Add(v);

			int32 id2 = (t % sides) + 1;

			triangles.Add(indexOffset);
			triangles.Add(id + indexOffset);
			triangles.Add(id2 + indexOffset);

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

		FVector bottomRight(location.X+x1* halfSize.X, location.Y + y1* halfSize.Y, location.Z);
		FVector topRight(location.X + x1 * halfSize.X, location.Y + y1 * halfSize.Y, location.Z+size.Z);
		FVector bottomLeft(location.X + x2 * halfSize.X, location.Y + y2 * halfSize.Y, location.Z);
		FVector topLeft(location.X + x2 * halfSize.X, location.Y + y2 * halfSize.Y, location.Z+size.Z);
		vertices.Add(bottomRight);	// id
		vertices.Add(topRight);		// id+1
		vertices.Add(bottomLeft);	// id+2
		vertices.Add(topLeft);		// id+3

		// bottom-right triangle
		triangles.Add(id + indexOffset);
		triangles.Add(id + 1 + indexOffset);
		triangles.Add(id + 2 + indexOffset);
		// top-left triangle
		triangles.Add(id + 2 + indexOffset);
		triangles.Add(id + 1 + indexOffset);
		triangles.Add(id + 3 + indexOffset);

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

			FVector v = FVector(location.X+x * halfSize.X, location.Y+ y * halfSize.Y, location.Z+size.Z);

			vertices.Add(v);

			//int32 id2 = centerID + (t % sides) +1;
			int32 id2 = id + 1;
			if (id2 > centerID + sides)
				id2 = centerID + 1;

			triangles.Add(centerID + indexOffset);
			triangles.Add(id2 + indexOffset);
			triangles.Add(id + indexOffset);

			normals.Add(FVector::UpVector);
			tangents.Add(FProcMeshTangent(FVector::ForwardVector, false));
			uv.Add(FVector2D((x + 1) / 2,
				(y + 1) / 2));

			++id;
		}
	}

}

static TArray<FVector2D> SphereUVs(float radius, int32 parallels, int32 pointsPerCircle)
{
    int32 size = parallels * pointsPerCircle;
    TArray<FVector2D> uvs;
    uvs.SetNum(size);

    float xStep = 1.0 / (pointsPerCircle - 1) /*aka meridians*/;
    float yStep = 1.0 / parallels;
    for (int32 cy = 0; cy < parallels; cy++)
    {
        for (int32 cx = 0; cx < pointsPerCircle; cx++)
        {
            uvs[--size] = FVector2D(xStep * cx, yStep * cy);
        }
    }

    return uvs;
}

static TArray<FVector> SphereAxisPoints(float radius, int32 parallels, float angleStep, const FVector& sphereOrigin)
{
	TArray<FVector> centers;
    centers.Reserve(parallels);

    for (int32 i = 0; i < parallels; ++i)
    {
        float currentAngle = i * angleStep;
        float unitHeight = FMath::Cos(currentAngle);
        centers.Add(FVector(sphereOrigin.X, sphereOrigin.Y, (unitHeight * radius)));
    }

    return centers;
}

static TArray<FVector> SphereNormals(const TArray<FVector>& vertices, const FVector& sphereOrigin)
{
	TArray<FVector> normals;

    for (const auto& vertex : vertices)
    {
        auto normal = vertex - FVector::ZeroVector;
        normal.Normalize();
        normals.Add(normal);
    }

    return normals;
}

void SphereMesh(Mesh& mesh, float radius, int32 parallels, int32 meridians)
{
	SphereMesh(mesh, radius, parallels, meridians, FVector::ZeroVector);
}

void SphereMesh(Mesh& mesh, float radius, int32 parallels, int32 meridians, const FVector& sphereOrigin)
{
	int32 pointsPerCircle = meridians + 1;
	int32 nVertices = pointsPerCircle * parallels;
	float parallelsAngleStep = PI / (parallels - 1);

	mesh.Allocate(nVertices, nVertices * 2);

	TArray<int32> ids;
    ids.Reserve(nVertices);

	// Find radius at a given distance from the sphere center on the sphere axis using pythagoras
	// After that create a circle at height `center.Z` with radius `localRadius`
    for (const auto& center : SphereAxisPoints(radius, parallels, parallelsAngleStep, sphereOrigin))
    {
        float localRadius = FMath::Sqrt((radius * radius) - (FMath::Abs(center.Z) * FMath::Abs(center.Z)));
        mesh.Vertices.Append(CircularNGonPoints(pointsPerCircle, localRadius, center, 0.0f));
    }

	// Just store ids sequentially for later use, here std::iota would've been useful...
	for (int32 id = 0; id < mesh.Vertices.Num(); ++id)
	{
		ids.Add(id);
	}

	// Given a top and a bottom circle, connect the two to form a strip made of triangles
    for (int32 circleStartingId = mesh.Vertices.Num() - pointsPerCircle;
            circleStartingId - pointsPerCircle >= 0;
            circleStartingId -= pointsPerCircle)
    {
        int32 bottomCircleStartingId = circleStartingId - pointsPerCircle;
        auto top = TArrayView<int32>(ids).Slice(circleStartingId, pointsPerCircle);
        auto bottom = TArrayView<int32>(ids).Slice(bottomCircleStartingId, pointsPerCircle);

        mesh.Triangles.Append(StripFromNGonCouple(top, bottom));
    }

    mesh.Normals.Append(SphereNormals(mesh.Vertices, sphereOrigin));
    mesh.UVs.Append(SphereUVs(radius, parallels, pointsPerCircle));
}

