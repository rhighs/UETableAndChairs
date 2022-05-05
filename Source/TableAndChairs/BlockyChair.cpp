#include "BlockyChair.h"

Mesh BlockyChair(FVector size)
{
	Mesh chairMesh;

	const float chairRadius = size.X / 2;

	const float restHeightRatio = 0.07f;
	const float backHeightRatio = 0.5f;
	const float legsHeightRatio = 0.4f;

	const float backLengthRatio = 0.2f;
	const float restLengthRatio = 1.0f;
	const float legsLengthRatio = 0.2f;

	const float backWidthRatio = 1.0f;
	const float restWidthRatio = 1.0f;
	const float legsWidthRatio = 0.2f;

	const FVector restSize(size.X * restWidthRatio, size.Y, size.Z * restHeightRatio);
	const FVector backSize(size.X * backWidthRatio, size.Y * backLengthRatio, size.Z * backHeightRatio);
	const FVector legsSize(size.X * legsWidthRatio, size.Y * legsLengthRatio, size.Z * legsHeightRatio);

	const FVector restPosition(0.0f,							     0.0f, 		 legsSize.Z + restSize.Z / 2);
	const FVector backPosition(0.0f,		-chairRadius + backSize.Y / 2, 	 restPosition.Z + backSize.Z / 2 + restSize.Z / 2);

	CubeMesh(chairMesh, restSize, restPosition);
	CubeMesh(chairMesh, backSize, backPosition);
	
	float legRadius = legsSize.X / 2;

	// Please don't touch this
	CubeMesh(chairMesh, legsSize, FVector(legRadius - chairRadius, legRadius - chairRadius, legsSize.Z / 2));
	CubeMesh(chairMesh, legsSize, FVector(- legRadius + chairRadius, legRadius - chairRadius, legsSize.Z / 2));
	CubeMesh(chairMesh, legsSize, FVector(legRadius - chairRadius, - legRadius + chairRadius, legsSize.Z / 2));
	CubeMesh(chairMesh, legsSize, FVector(- legRadius + chairRadius, - legRadius + chairRadius, legsSize.Z / 2));

	return chairMesh;
}

