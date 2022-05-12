// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Resizable.h"
#include "TableWithChairsGroup.h"
#include "MousePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TABLEANDCHAIRS_API AMousePlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	bool bMoveCamera;
	bool bCornerPressed;
public:
	AMousePlayerController();

	// Called every frame
	//virtual void Tick(float DeltaTime) override;
protected:
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;
private:
	void OnMoveCameraPressed();
	void OnMoveCameraReleased();
	void LeftButtonClicked();
	void LeftButtonReleased();
	FVector2D _findCoordinatesInZPlane(float Zvalue, FVector location, FVector direction);

	IResizable* _resizableActor;
	FVector _hitLocation;
};
