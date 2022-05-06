// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Mesh.h"
#include "TableWithChairsGroup.generated.h"

UCLASS()
class TABLEANDCHAIRS_API ATableWithChairsGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATableWithChairsGroup();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& e) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UProceduralMeshComponent* _proceduralMesh;
	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterialInterface* _material;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "20", ClampMax = "1000", UIMin = "20", UIMax = "1000"))
		float _xLength = 20;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "20", ClampMax = "500", UIMin = "20", UIMax = "500"))
		float _yLenght = 20;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "15", ClampMax = "100", UIMin = "15", UIMax = "100"))
		uint32 _height = 15;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "3", ClampMax = "30", UIMin = "3", UIMax = "30"))
		uint32 _tableSides = 3;

private:
	FVector _tableSize;
	float _spaceBetweenChairs;
	FVector _chairSize;
	int32 _numberOfChairs;

	void BuildTable(Mesh& mesh, FVector size);
	void BuildChair(Mesh& mesh, FVector heightAndPosition);

	void _updateTable();
	void _placeChairs();
};
