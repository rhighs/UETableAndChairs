// Fill out your copyright notice in the Description page of Project Settings.


#include "MousePlayerController.h"

AMousePlayerController::AMousePlayerController()
{

	bMoveCamera = false;
	bLeftButtonPressed = false;
	// probably they're all true by default
#if 1 
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
#endif

	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMousePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("LeftButtonMouse", IE_Pressed, this, &AMousePlayerController::LeftButtonClicked);
	//InputComponent->BindAction("LeftButtonMouse", IE_Released, this, &AMousePlayerController::LeftButtonReleased);

	InputComponent->BindAction("OnMoveCamera", IE_Pressed, this, &AMousePlayerController::OnMoveCameraPressed);
	InputComponent->BindAction("OnMoveCamera", IE_Released, this, &AMousePlayerController::OnMoveCameraReleased);
}

void AMousePlayerController::OnMoveCameraPressed()
{
	bMoveCamera = true;
}

void AMousePlayerController::OnMoveCameraReleased()
{
	bMoveCamera = false;
}

void AMousePlayerController::LeftButtonClicked()
{
	bLeftButtonPressed = true;
}

void AMousePlayerController::LeftButtonReleased()
{
	bLeftButtonPressed = false;
}

/*
tick function:
	if(bLeftButtonPressed):
		get mouse position in 3D space;
		update table size;
		redraw table;
*/