// Fill out your copyright notice in the Description page of Project Settings.

#include "TableWithChairsGroup.h"


// Sets default values
ATableWithChairsGroup::ATableWithChairsGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_proceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	RootComponent = _proceduralMesh;

	_material = CreateDefaultSubobject<UMaterialInterface>(TEXT("Material"));

	SetSize(FVector(50, 30, 30));
	_chairSize = FVector(20, 20, GetSize().Z/2);
	_spaceBetweenChairs = 3;
	float chairOccupiedWidth = _chairSize.X + 2 * _spaceBetweenChairs;
	_numberOfChairs = 2 * (GetSize().X / chairOccupiedWidth)
					+ 2 * (GetSize().Y / chairOccupiedWidth);
	CornerSphereComponents.Reserve(4);
	for (int i=0; i< 4; ++i)
	{

		FString j = "Corner" + i;//FString::FromInt(i);
		CornerSphereComponents.Add(CreateDefaultSubobject<USphereComponent>(FName(*j)));

		CornerSphereComponents[i]->SetupAttachment(_proceduralMesh);
		CornerSphereComponents[i]->SetSphereRadius(3);
	}

	ChairsSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Chairs Path Spline"));
	ChairsSplineComponent->SetupAttachment(_proceduralMesh);
}


FVector ATableWithChairsGroup::GetResizableLocation()
{
	FVector location = GetActorLocation();
	return location;
}


void ATableWithChairsGroup::Resize(const FVector& newSize)
{
	SetSize(newSize);

	Mesh table{};
	BuildRectangularTable(table, newSize);
	bool createCollision = false;
	_proceduralMesh->CreateMeshSection_LinearColor(0, table.Vertices, table.Triangles, table.Normals, table.UVs, {}, table.Tangents, createCollision);

	_putCornersInPlace();
	_updateSpline();
	_placeChairs();
}

void ATableWithChairsGroup::BuildRectangularTable(Mesh& mesh, FVector size)
{
	Mesh& table = mesh;
	float sqrt_2 = 1.41421356237;
	BuildCylinder(table, 4, FVector(sqrt_2 * size.X, sqrt_2 * size.Y, 3), FVector(0, 0, size.Z), true, true);
	float padding = 0.8;
	BuildCylinder(table, 6, FVector(2, 2, size.Z), FVector(-padding*size.X / 2, -padding*size.Y / 2, 0), false, true);
	BuildCylinder(table, 6, FVector(2, 2, size.Z), FVector( padding*size.X / 2, -padding*size.Y / 2, 0), false, true);
	BuildCylinder(table, 6, FVector(2, 2, size.Z), FVector(-padding*size.X / 2,  padding*size.Y / 2, 0), false, true);
	BuildCylinder(table, 6, FVector(2, 2, size.Z), FVector( padding*size.X / 2,  padding*size.Y / 2, 0), false, true);

	_shapeCorners.Add(FVector(-size.X / 2,  size.Y / 2, 0));
	_shapeCorners.Add(FVector( size.X / 2,  size.Y / 2, 0));
	_shapeCorners.Add(FVector( size.X / 2, -size.Y / 2, 0));
	_shapeCorners.Add(FVector(-size.X / 2, -size.Y / 2, 0));
}

/// <summary>
/// 
/// </summary>
/// <param name="mesh"></param>
/// <param name="positionAndHeight">x and y define the position; z is the height</param>
void ATableWithChairsGroup::BuildChair(Mesh& mesh, FVector positionAndHeight)
{
	FVector size = FVector(_chairSize.X, _chairSize.Y, _chairSize.Z);
	FVector pos = FVector(positionAndHeight.X, _distanceChairTable, 0);
	Mesh& table = mesh;
	float sqrt_2 = 1.41421356237;
	BuildCylinder(table, 4, FVector(sqrt_2*size.X, sqrt_2*size.Y, 3), FVector(pos.X, pos.Y, size.Z), true, true);
	float padding = 0.8;
	BuildCylinder(table, 5, FVector(2, 2, size.Z), FVector(pos.X -padding*size.X / 2, pos.Y -padding*size.Y / 2, 0), false, true);
	BuildCylinder(table, 5, FVector(2, 2, size.Z), FVector(pos.X +padding*size.X / 2, pos.Y -padding*size.Y / 2, 0), false, true);
	BuildCylinder(table, 5, FVector(2, 2, size.Z), FVector(pos.X -padding*size.X / 2, pos.Y +padding*size.Y / 2, 0), false, true);
	BuildCylinder(table, 5, FVector(2, 2, size.Z), FVector(pos.X +padding*size.X / 2, pos.Y +padding*size.Y / 2, 0), false, true);
}

#if WITH_EDITOR
void ATableWithChairsGroup::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	bool xLengthHasChanged = PropertyName == GET_MEMBER_NAME_CHECKED(ATableWithChairsGroup, _xLength);
	bool yLengthHasChanged = PropertyName == GET_MEMBER_NAME_CHECKED(ATableWithChairsGroup, _yLenght);
	bool heigthHasChanged = PropertyName == GET_MEMBER_NAME_CHECKED(ATableWithChairsGroup, _height);
	bool sidesHasChanged = PropertyName == GET_MEMBER_NAME_CHECKED(ATableWithChairsGroup, _tableSides);

	if (xLengthHasChanged || yLengthHasChanged || heigthHasChanged || sidesHasChanged)
	{
		Resize(FVector(_xLength, _yLenght, _height));
	}

	bool materialHasChanged = PropertyName == GET_MEMBER_NAME_CHECKED(ATableWithChairsGroup, _material);
	if (materialHasChanged)
	{
		_proceduralMesh->SetMaterial(0, _material);
	}

	Super::PostEditChangeProperty(e);
}
#endif


// Called when the game starts or when spawned
void ATableWithChairsGroup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATableWithChairsGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATableWithChairsGroup::_updateTable()
{
	Mesh table{};
	const FVector tableSize = GetSize();
	BuildRectangularTable(table, tableSize);

	bool createCollision = false;
	_proceduralMesh->CreateMeshSection_LinearColor(0, table.Vertices, table.Triangles, table.Normals, table.UVs, {}, table.Tangents, createCollision);

	_placeChairs();
}

void ATableWithChairsGroup::_putCornersInPlace()
{
	static TArray<FVector> quadrantsSigns{ FVector(1,  1, 1),
										 FVector(-1,  1, 1),
										 FVector(-1, -1, 1),
										 FVector(1, -1, 1) };

	if (CornerSphereComponents.Num() != quadrantsSigns.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("corner hitboxes can't be positioned"));
		return;
	}

	int32 id = 0;
	for (auto& corner : CornerSphereComponents)
	{
		FVector sphereLocation = FVector(GetSize().X / 2, GetSize().Y / 2, GetSize().Z) * quadrantsSigns[id];
		corner->SetRelativeLocation(sphereLocation);

		++id;
	}
}

void ATableWithChairsGroup::_updateSpline()
{
	ChairsSplineComponent->ClearSplinePoints();
	// clockwise arrangement of points
	static TArray<FVector> quadrantsSigns{ FVector(-1, -1, 1),
										   FVector( 1, -1, 1),
										   FVector( 1,  1, 1),
										   FVector(-1,  1, 1),
										   FVector(-1, -1, 1) };
	FVector lineSize = GetSize()/2 + FVector(_distanceChairTable, _distanceChairTable, 0);
	lineSize.Z = 0;

	int id = 0;
	for (const auto& signs : quadrantsSigns)
	{
		ChairsSplineComponent->AddSplinePoint(GetActorLocation() + lineSize * signs, ESplineCoordinateSpace::Local, false);
		ChairsSplineComponent->SetSplinePointType(id, ESplinePointType::Linear);
		++id;
	}
}

void ATableWithChairsGroup::_placeChairs()
{
	float chairOccupiedWidth = _chairSize.X + 2 * _spaceBetweenChairs;
	float currentDistance = _distanceChairTable+chairOccupiedWidth/2;
	FVector tablePosition = GetActorLocation();
	FVector chairPosAndHeight = FVector::ZeroVector; 
	//float chairHeight = _chairSize.Z;
	Mesh firstChair{};
	//BuildChair(firstChair, FVector(chairPosition, chairHeight));

	float splineLength = ChairsSplineComponent->GetSplineLength();
	float distancePoint1 = ChairsSplineComponent->GetDistanceAlongSplineAtSplinePoint(1);
	while (currentDistance <= (distancePoint1 - _distanceChairTable) )
	{
		chairPosAndHeight = ChairsSplineComponent->GetLocationAtDistanceAlongSpline(currentDistance, ESplineCoordinateSpace::Local);
		chairPosAndHeight.Z = _chairSize.Z;
		BuildChair(firstChair, chairPosAndHeight);

		currentDistance += chairOccupiedWidth;
	}

	bool createCollision = false;
	_proceduralMesh->CreateMeshSection_LinearColor(1, firstChair.Vertices, firstChair.Triangles, firstChair.Normals,
												   firstChair.UVs, {}, firstChair.Tangents, createCollision);
	_proceduralMesh->SetMaterial(1, _material);
}

void ATableWithChairsGroup::OnConstruction(const FTransform& Transform)
{
	_updateTable();
	_proceduralMesh->SetMaterial(0, _material);

	for (auto& corner : CornerSphereComponents)
	{
		corner->SetSphereRadius(3);
		corner->SetCollisionResponseToAllChannels(ECR_Block);
	}

	_putCornersInPlace();
	_updateSpline();
	ChairsSplineComponent->SetDrawDebug(true);
	SetActorEnableCollision(true);
}