#include "TableResizerController.h"
#include "TestActor.h"

#include "DrawDebugHelpers.h"

ATableResizerController::ATableResizerController()
	: hitPoint(FVector::ZeroVector), releasePoint(FVector::ZeroVector)
{
	bShowMouseCursor = true;
	UE_LOG(LogTemp, Warning, TEXT("PlayerController Ctor called"));
}

void ATableResizerController::ExitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}

void ATableResizerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("PlayerController BeginPlay called"));

	InputComponent->BindAction("MouseLeftClicked", IE_Pressed, this, &ATableResizerController::LeftButtonClicked);
	InputComponent->BindAction("MouseLeftClicked", IE_Released, this, &ATableResizerController::LeftButtonReleased);
	InputComponent->BindAction("Escape", IE_Pressed, this, &ATableResizerController::ExitGame);
}

void ATableResizerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void ATableResizerController::LeftButtonClicked()
{
	hitPoint = PerformRaycast();
}

void ATableResizerController::UpdateRotation(float deltaTime)
{
	Super::UpdateRotation(deltaTime);
}

FVector ATableResizerController::PerformRaycast()
{
	FHitResult hitResult;
	GetHitResultUnderCursor(ECC_Pawn, false, hitResult);

	if (hitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("SOMETHING GOT HIT: %s"), *(AActor::GetDebugName(hitResult.GetActor())));
		FVector vec(hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z);
		DrawDebugPoint(GetWorld(), vec, 100.0f, FColor::Red, false, 10.0f, 1);

		return vec;
	}

	return FVector::ZeroVector;
}

void ATableResizerController::LeftButtonReleased()
{
}

