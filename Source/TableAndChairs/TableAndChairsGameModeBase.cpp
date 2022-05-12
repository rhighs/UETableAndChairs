// Copyright Epic Games, Inc. All Rights Reserved.


#include "TableAndChairsGameModeBase.h"
#include "TableResizerController.h"

ATableAndChairsGameModeBase::ATableAndChairsGameModeBase()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ATableResizerController::StaticClass();
}

void ATableAndChairsGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATableAndChairsGameModeBase::StartPlay()
{
	Super::StartPlay();
}

