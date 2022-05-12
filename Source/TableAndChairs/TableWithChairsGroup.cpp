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
	_chairSize = FVector(10, 10, GetSize().Z / 2);
	_spaceBetweenChairs = 5;
	float chairOccupiedWidth = _chairSize.X + 2 * _spaceBetweenChairs;
	_numberOfChairs = 2 * (GetSize().X / chairOccupiedWidth)
					+ 2 * (GetSize().Y / chairOccupiedWidth);
	CornerSphereComponents.Reserve(4);
	for (int i=0; i< 4; ++i)
	{
		FString j = "Corner" + i;
		CornerSphereComponents.Add(CreateDefaultSubobject<USphereComponent>(FName(*j)));

		CornerSphereComponents[i]->SetupAttachment(_proceduralMesh);
	}
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
	_placeChairs();
}

void ATableWithChairsGroup::BuildRectangularTable(Mesh& mesh, FVector size)
{
	Mesh& table = mesh;
	CubeMesh(table, FVector(size.X, size.Y, 3), FVector(0, 0, size.Z));
	BuildCylinder(table, 4, FVector(2, 2, size.Z), FVector(-size.X / 2, -size.Y / 2, 0), false, true);
	BuildCylinder(table, 4, FVector(2, 2, size.Z), FVector(size.X / 2, -size.Y / 2, 0), false, true);
	BuildCylinder(table, 4, FVector(2, 2, size.Z), FVector(-size.X / 2, size.Y / 2, 0), false, true);
	BuildCylinder(table, 4, FVector(2, 2, size.Z), FVector(size.X / 2, size.Y / 2, 0), false, true);

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
	CubeMesh(table, FVector(size.X, size.Y, 3), FVector(pos.X, pos.Y, size.Z));
	BuildCylinder(table, 4, FVector(2, 2, size.Z), FVector(pos.X -size.X / 2, pos.Y -size.Y / 2, 0), false, true);
	BuildCylinder(table, 4, FVector(2, 2, size.Z), FVector(pos.X +size.X / 2, pos.Y -size.Y / 2, 0), false, true);
	BuildCylinder(table, 4, FVector(2, 2, size.Z), FVector(pos.X -size.X / 2, pos.Y+size.Y / 2, 0), false, true);
	BuildCylinder(table, 4, FVector(2, 2, size.Z), FVector(pos.X +size.X / 2, pos.Y+size.Y / 2, 0), false, true);
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
	static TArray<FVector> quadrantsDir{ FVector(1,  1, 1),
										 FVector(-1,  1, 1),
										 FVector(-1, -1, 1),
										 FVector(1, -1, 1) };

	if (CornerSphereComponents.Num() != quadrantsDir.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("corner hitboxes can't be positioned"));
		return;
	}

	int32 id = 0;
	for (auto& corner : CornerSphereComponents)
	{
		FVector sphereLocation = FVector(GetSize().X / 2, GetSize().Y / 2, GetSize().Z) * quadrantsDir[id];
		corner->SetRelativeLocation(sphereLocation);

		++id;
	}
}

void ATableWithChairsGroup::_placeChairs()
{
	/*
	if(chairWidth <= side length
	*/

	int32 maxSeats = _maxSeatsCount();
	for (int32 chairCount = 0; chairCount < maxSeats; ++chairCount)
	{
		FVector seatPosition{};
		FVector seatNormal{};
		_getPointAndNormalInPerimeter(float(chairCount)/ maxSeats, seatPosition, seatNormal);
	}
	// ...
	// ...
	// ...

	FVector2D chairPosition = FVector2D(-GetSize().X / 2, -GetSize().Y / 2);
	float chairHeight = _chairSize.Z;
	Mesh firstChair{};
	BuildChair(firstChair, FVector(chairPosition, chairHeight));

	bool createCollision = false;
	_proceduralMesh->CreateMeshSection_LinearColor(1, firstChair.Vertices, firstChair.Triangles, firstChair.Normals,
												   firstChair.UVs, {}, firstChair.Tangents, createCollision);
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

	SetActorEnableCollision(true);
}