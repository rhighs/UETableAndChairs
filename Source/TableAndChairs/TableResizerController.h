#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProceduralMeshComponent.h"
#include "TableResizerController.generated.h"

UCLASS()
class TABLEANDCHAIRS_API ATableResizerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATableResizerController();

	void LeftButtonClicked();
	void LeftButtonReleased();
	void ExitGame();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
};
