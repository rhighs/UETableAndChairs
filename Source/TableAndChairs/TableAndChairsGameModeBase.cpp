// Copyright Epic Games, Inc. All Rights Reserved.


#include "TableAndChairsGameModeBase.h"

//#include "PlayerPawn.h"
#include "MousePlayerController.h"

ATableAndChairsGameModeBase::ATableAndChairsGameModeBase()
{
	PlayerControllerClass = AMousePlayerController::StaticClass();
}

void ATableAndChairsGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

