// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "SphericalHitBoxComponent.generated.h"

UCLASS(Blueprintable)
class TABLEANDCHAIRS_API USphericalHitBoxComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:	
	USphericalHitBoxComponent(const FObjectInitializer& objectInitializer);

	void Build(float radius, int32 parallels, int32 meridians, const FVector& sphereOrigin, bool collisionsEnabled);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
