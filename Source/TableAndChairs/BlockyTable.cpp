// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockyTable.h"
#include "Mesh.h"

Mesh BlockyTable(const FVector& size)
{
	Mesh tableMesh;

	const float chairRadius = size.X / 2;

	const float topHeightRatio = 0.2f;
	const float legsHeightRatio = 0.8f;

	const float topWidthRatio = 1.0f;
	const float legsWidthRatio = 0.3f;

	const float topLengthRatio = 1.0f;
	const float legsLengthRatio = 0.3f;

	FVector topSize(size.X * topWidthRatio, size.Y * topLengthRatio, size.Z * topHeightRatio);
    FVector legsSize(size.X * legsWidthRatio, size.Y * legsLengthRatio, size.Z * legsHeightRatio);

    if (legsSize.Y >= 50.0f) {
        legsSize.Y = 50.0f;
    }

    if (legsSize.X >= 50.0f) {
        legsSize.X = 50.0f;
    }

    FVector topPosition(0.0f, 0.0f, size.Z - topSize.Z / 2);

	CubeMesh(tableMesh, topSize, topPosition);

    const float halfLegX = legsSize.X / 2;
    const float halfLegY = legsSize.Y / 2;
    const float halfTopX = topSize.X / 2;
    const float halfTopY = topSize.Y / 2;
	CubeMesh(tableMesh, legsSize, FVector(halfLegX - halfTopX, halfLegY - halfTopY, legsSize.Z / 2));
	CubeMesh(tableMesh, legsSize, FVector(- halfLegX + halfTopX, halfLegY - halfTopY, legsSize.Z / 2));
	CubeMesh(tableMesh, legsSize, FVector(halfLegX - halfTopX, - halfLegY + halfTopY, legsSize.Z / 2));
	CubeMesh(tableMesh, legsSize, FVector(- halfLegX + halfTopX, - halfLegY + halfTopY, legsSize.Z / 2));

	return tableMesh;
}

TArray<TTuple<FVector, FVector>> ChairPositions(const FVector& tableSize, const FVector& chairSize)
{
	TArray<TTuple<FVector, FVector>> positions;
	int32 initialAllocation = ((tableSize.X / chairSize.X) / 2 + (tableSize.Y / chairSize.Y) / 2) * 2;
	positions.Reserve(initialAllocation);

	const float gapBetweenChairs = 30.0f;

	const float tableSideX = tableSize.X / 2 - chairSize.X;
	const float tableSideY = tableSize.Y / 2 - chairSize.X;

	const float distanceFromTableX = (gapBetweenChairs + chairSize.X / 2) * 2;
	const float distanceFromTableY = (gapBetweenChairs + chairSize.X / 2) * 2;
	const float chairsSideX = tableSideX + distanceFromTableX;
	const float chairsSideY = tableSideY + distanceFromTableY;

	for (float x = -tableSideX; x < tableSideX; x += chairSize.X + gapBetweenChairs)
	{
		auto v1 = FVector(x, chairsSideY, 0.0f);
		auto v2 = FVector(x, -chairsSideY, 0.0f);

		positions.Add(MakeTuple(v1, FVector(0.0, 180.0, 0.0)));
		positions.Add(MakeTuple(v2, FVector::ZeroVector));
	}

	for (float y = -tableSideY; y < tableSideY; y += chairSize.X + gapBetweenChairs)
	{
		auto v1 = FVector(chairsSideX, y, 0.0f);
		auto v2 = FVector(-chairsSideX, y, 0.0f);

		positions.Add(MakeTuple(v1, FVector(0.0, 90.0, 0.0)));
		positions.Add(MakeTuple(v2, FVector(0.0, 270.0, 0.0)));
	}

	return positions;
}

