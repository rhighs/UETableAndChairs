// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
	bool bLeftButtonPressed;
public:
	AMousePlayerController();

protected:
	virtual void SetupInputComponent() override;

private:
	void OnMoveCameraPressed();
	void OnMoveCameraReleased();
	void LeftButtonClicked();
	void LeftButtonReleased();
};