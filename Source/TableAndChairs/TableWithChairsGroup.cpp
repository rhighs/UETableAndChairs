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

	_tableSize = FVector(50, 40, 30);
	_chairSize = FVector(10, 10, _tableSize.Z/2);
	_spaceBetweenChairs = 5;
	float chairOccupiedWidth = _chairSize.X + 2 * _spaceBetweenChairs;
	_numberOfChairs = 2 * (_tableSize.X / chairOccupiedWidth)
					+ 2 * (_tableSize.Y / chairOccupiedWidth);
}

void ATableWithChairsGroup::BuildTable(Mesh& mesh, FVector size)
{
	Mesh& table = mesh;
	BuildCylinder(table, 4, FVector(size.X, size.Y, 3), FVector(0,0,size.Z), true, true);
	Mesh bottomLeftLeg{};
	BuildCylinder(bottomLeftLeg, 4, FVector(2, 2, size.Z), FVector(-size.X/2,-size.Y/2,0), false, true);
	Mesh bottomRightLeg{};
	BuildCylinder(bottomRightLeg, 4, FVector(2, 2, size.Z), FVector(size.X / 2, -size.Y / 2,0), false, true);
	Mesh topLeftLeg{};
	BuildCylinder(topLeftLeg, 4, FVector(2, 2, size.Z), FVector(-size.X / 2, size.Y / 2, 0), false, true);
	Mesh topRightLeg{};
	BuildCylinder(topRightLeg, 4, FVector(2, 2, size.Z), FVector(size.X / 2, size.Y / 2, 0), false, true);

#if 0
	table.Vertices.Append(bottomLeftLeg.Vertices);
	table.Vertices.Append(bottomRightLeg.Vertices);
	table.Vertices.Append(topLeftLeg.Vertices);
	table.Vertices.Append(topRightLeg.Vertices);

	table.Normals.Append(bottomLeftLeg.Normals);
	table.Normals.Append(bottomRightLeg.Normals);
	table.Normals.Append(topLeftLeg.Normals);
	table.Normals.Append(topRightLeg.Normals);

	table.UVs.Append(bottomLeftLeg.UVs);
	table.UVs.Append(bottomRightLeg.UVs);
	table.UVs.Append(topLeftLeg.UVs);
	table.UVs.Append(topRightLeg.UVs);

	table.Tangents.Append(bottomLeftLeg.Tangents);
	table.Tangents.Append(bottomRightLeg.Tangents);
	table.Tangents.Append(topLeftLeg.Tangents);
	table.Tangents.Append(topRightLeg.Tangents);
#else
	int32 startingID = table.Vertices.Num();
	for (auto& v : bottomLeftLeg.Vertices) table.Vertices.Add(v);
	for (int32& id : bottomLeftLeg.Triangles)
		table.Triangles.Add(startingID + id);

	startingID = table.Vertices.Num();
	for (auto& v : bottomRightLeg.Vertices) table.Vertices.Add(v);
	for (int32& id : bottomRightLeg.Triangles)
		table.Triangles.Add(startingID + id);

	startingID = table.Vertices.Num();
	for (auto& v : topLeftLeg.Vertices) table.Vertices.Add(v);
	for (int32& id : topLeftLeg.Triangles)
		table.Triangles.Add(startingID + id);

	startingID = table.Vertices.Num();
	for (auto& v : topRightLeg.Vertices) table.Vertices.Add(v);
	for (int32& id : topRightLeg.Triangles)
		table.Triangles.Add(startingID + id);

	for (auto& n : bottomLeftLeg.Normals) table.Normals.Add(n);
	for (auto& n : bottomRightLeg.Normals) table.Normals.Add(n);
	for (auto& n : topLeftLeg.Normals) table.Normals.Add(n);
	for (auto& n : topRightLeg.Normals) table.Normals.Add(n);

	for (auto& uv : bottomLeftLeg.UVs) table.UVs.Add(uv);
	for (auto& uv : bottomRightLeg.UVs) table.UVs.Add(uv);
	for (auto& uv : topLeftLeg.UVs) table.UVs.Add(uv);
	for (auto& uv : topRightLeg.UVs) table.UVs.Add(uv);

	for (auto& t : bottomLeftLeg.Tangents) table.Tangents.Add(t);
	for (auto& t : bottomRightLeg.Tangents) table.Tangents.Add(t);
	for (auto& t : topLeftLeg.Tangents) table.Tangents.Add(t);
	for (auto& t : topRightLeg.Tangents) table.Tangents.Add(t);
#endif

}


void ATableWithChairsGroup::BuildChair(Mesh& mesh, FVector heightAndPosition)
{
	FVector size = FVector(_chairSize.X, _chairSize.Y, _chairSize.Z);
	FVector pos = FVector(heightAndPosition.X, heightAndPosition.Y - size.Y, 0);
	Mesh& table = mesh;
	BuildCylinder(table, 4, FVector(size.X, size.Y, 3), FVector(pos.X, pos.Y, size.Z), true, true);
	Mesh bottomLeftLeg{};
	BuildCylinder(bottomLeftLeg, 4, FVector(2, 2, size.Z), FVector(-size.X / 2, -size.Y / 2, 0), false, true);
	Mesh bottomRightLeg{};
	BuildCylinder(bottomRightLeg, 4, FVector(2, 2, size.Z), FVector(size.X / 2, -size.Y / 2, 0), false, true);
	Mesh topLeftLeg{};
	BuildCylinder(topLeftLeg, 4, FVector(2, 2, size.Z), FVector(-size.X / 2, size.Y / 2, 0), false, true);
	Mesh topRightLeg{};
	BuildCylinder(topRightLeg, 4, FVector(2, 2, size.Z), FVector(size.X / 2, size.Y / 2, 0), false, true);

	int32 startingID = table.Vertices.Num();
	for (auto& v : bottomLeftLeg.Vertices) table.Vertices.Add(v + pos);
	for (int32& id : bottomLeftLeg.Triangles)
		table.Triangles.Add(startingID + id);

	startingID = table.Vertices.Num();
	for (auto& v : bottomRightLeg.Vertices) table.Vertices.Add(v + pos);
	for (int32& id : bottomRightLeg.Triangles)
		table.Triangles.Add(startingID + id);

	startingID = table.Vertices.Num();
	for (auto& v : topLeftLeg.Vertices) table.Vertices.Add(v + pos);
	for (int32& id : topLeftLeg.Triangles)
		table.Triangles.Add(startingID + id);

	startingID = table.Vertices.Num();
	for (auto& v : topRightLeg.Vertices) table.Vertices.Add(v + pos);
	for (int32& id : topRightLeg.Triangles)
		table.Triangles.Add(startingID + id);

	for (auto& n : bottomLeftLeg.Normals) table.Normals.Add(n);
	for (auto& n : bottomRightLeg.Normals) table.Normals.Add(n);
	for (auto& n : topLeftLeg.Normals) table.Normals.Add(n);
	for (auto& n : topRightLeg.Normals) table.Normals.Add(n);

	for (auto& uv : bottomLeftLeg.UVs) table.UVs.Add(uv);
	for (auto& uv : bottomRightLeg.UVs) table.UVs.Add(uv);
	for (auto& uv : topLeftLeg.UVs) table.UVs.Add(uv);
	for (auto& uv : topRightLeg.UVs) table.UVs.Add(uv);

	for (auto& t : bottomLeftLeg.Tangents) table.Tangents.Add(t);
	for (auto& t : bottomRightLeg.Tangents) table.Tangents.Add(t);
	for (auto& t : topLeftLeg.Tangents) table.Tangents.Add(t);
	for (auto& t : topRightLeg.Tangents) table.Tangents.Add(t);
}

#if WITH_EDITOR
void ATableWithChairsGroup::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	bool xLengthHasChanged = PropertyName == GET_MEMBER_NAME_CHECKED(ATableWithChairsGroup, _xLength);
	bool yLengthHasChanged = PropertyName == GET_MEMBER_NAME_CHECKED(ATableWithChairsGroup, _yLenght);
	bool heigthHasChanged = PropertyName == GET_MEMBER_NAME_CHECKED(ATableWithChairsGroup, _height);

	if (xLengthHasChanged || yLengthHasChanged || heigthHasChanged)
	{
		_tableSize.X = _xLength;
		_tableSize.Y = _yLenght;
		_tableSize.Z = _height;
		
		_updateTable();
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
	BuildTable(table, _tableSize);

	bool createCollision = false;
	_proceduralMesh->CreateMeshSection_LinearColor(0, table.Vertices, table.Triangles, table.Normals, table.UVs, {}, table.Tangents, createCollision);

	_placeChairs();
}

void ATableWithChairsGroup::_placeChairs()
{
	FVector2D chairPosition = FVector2D(-_tableSize.X / 2, -_tableSize.Y / 2);
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
}