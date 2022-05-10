// Copyright Epic Games, Inc. All Rights Reserved.


#include "TableAndChairsGameModeBase.h"

#include "PlayerPawn.h"
#include "TableResizerController.h"

ATableAndChairsGameModeBase::ATableAndChairsGameModeBase()
{
	PlayerControllerClass = ATableResizerController::StaticClass();
}

void ATableAndChairsGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

