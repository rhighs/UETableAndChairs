#include "BlockyChair.h"

Mesh BlockyChair(FVector size)
{
	Mesh chairMesh;

	const float restHeightRatio = 0.07f;
	const float backHeightRatio = 0.5f;
	const float legsHeightRatio = 0.4f;

	const float backLengthRatio = 0.2f;
	const float restLengthRatio = 1.0f;
	const float legsLengthRatio = 0.2f;

	const float backWidthRatio = 1.0f;
	const float restWidthRatio = 1.0f;
	const float legsWidthRatio = 0.2f;

	FVector restSize(size.X * restWidthRatio, size.Y, size.Z * restHeightRatio);
	FVector backSize(size.X * backWidthRatio, size.Y * backLengthRatio, size.Z * backHeightRatio);
	FVector legsSize(size.X * legsWidthRatio, size.Y * legsLengthRatio, size.Z * legsHeightRatio);

    if (legsSize.Y >= 50.0f) {
        legsSize.Y = 50.0f;
        backSize.Y = 50.0f;
    }

    if (legsSize.X >= 50.0f) {
        legsSize.X = 50.0f;
    }

	FVector restPosition(0.0f, 0.0f, legsSize.Z + restSize.Z / 2);
	FVector backPosition(0.0f, -restSize.Y / 2 + backSize.Y / 2, restPosition.Z + backSize.Z / 2 + restSize.Z / 2);
	
	CubeMesh(chairMesh, restSize, restPosition);
	CubeMesh(chairMesh, backSize, backPosition);

	// Please don't touch this
    const float halfLegX = legsSize.X / 2;
    const float halfLegY = legsSize.Y / 2;
    const float halfRestX = restSize.X / 2;
    const float halfRestY = restSize.Y / 2;
	CubeMesh(chairMesh, legsSize, FVector(halfLegX - halfRestX, halfLegY - halfRestY, legsSize.Z / 2));
	CubeMesh(chairMesh, legsSize, FVector(- halfLegX + halfRestX, halfLegY - halfRestY, legsSize.Z / 2));
	CubeMesh(chairMesh, legsSize, FVector(halfLegX - halfRestX, - halfLegY + halfRestY, legsSize.Z / 2));
	CubeMesh(chairMesh, legsSize, FVector(- halfLegX + halfRestX, - halfLegY + halfRestY, legsSize.Z / 2));

	return chairMesh;
}

