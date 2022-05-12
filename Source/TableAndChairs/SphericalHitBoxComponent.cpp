#include "SphericalHitBoxComponent.h"
#include "Mesh.h"

USphericalHitBoxComponent::USphericalHitBoxComponent(const FObjectInitializer& objectInitializer)
	: UProceduralMeshComponent(objectInitializer)
{
	bUseComplexAsSimpleCollision = true;
	bUseAsyncCooking = true;
}

void USphericalHitBoxComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USphericalHitBoxComponent::Build(float radius, int32 parallels, int32 meridians, const FVector& sphereOrigin, bool collisionsEnabled)
{
	Mesh mesh;
	SphereMesh(mesh, radius, parallels, meridians, sphereOrigin);

	CreateMeshSection_LinearColor(
		0,
		mesh.Vertices,
		mesh.Triangles,
		mesh.Normals,
		mesh.UVs,
		TArray<FLinearColor>(),
		mesh.Tangents,
		collisionsEnabled
	);
}

void USphericalHitBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
