#include "BlockyChair.h"

Mesh BlockyChair(FVector size)
{
	Mesh chairMesh;

	const float restHeightRatio = 0.1f;
	const float backHeightRatio = 0.5f;
	const float legsHeightRatio = 0.4f;

	const float backLengthRatio = 0.2f;
	const float restLengthRatio = 1.0f;
	const float legsLengthRatio = 0.2f;

	const float backWidthRatio = 1.0f;
	const float restWidthRatio = 1.0f;
	const float legsWidthRatio = 0.2f;

	const FVector restSize(size.X * restWidthRatio, size.Y * restLengthRatio, size.Z * restHeightRatio);
	const FVector backSize(size.X * backWidthRatio, size.Y * backLengthRatio, size.Z * backHeightRatio);
	const FVector legsSize(size.X * legsWidthRatio, size.Y * legsLengthRatio, size.Z * legsHeightRatio);

	CubeMesh(chairMesh, restSize);
	CubeMesh(chairMesh, backSize);

	for (size_t i = 0; i < 4; i++)
	{
		CubeMesh(chairMesh, legsSize, FVector::ZeroVector /*<- Should be some offset*/);
	}

	return chairMesh;
}

