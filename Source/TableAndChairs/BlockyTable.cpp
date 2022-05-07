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
