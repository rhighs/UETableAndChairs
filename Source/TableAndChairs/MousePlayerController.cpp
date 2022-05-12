// Fill out your copyright notice in the Description page of Project Settings.

#include "MousePlayerController.h"
#include "DrawDebugHelpers.h"

void PrintDebugMessage(const FString& message, float displayTime = 10.f, FColor textColor = FColor::Yellow, int key = -1)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(key, displayTime, textColor, message);
	}
}

AMousePlayerController::AMousePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bMoveCamera = false;
	bCornerPressed = false;

#if 1 
	// probably they're all true by default
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
#endif

	DefaultMouseCursor = EMouseCursor::Crosshairs;
}


void AMousePlayerController::PlayerTick(float DeltaTime)
{

	Super::PlayerTick(DeltaTime);
	if (!bCornerPressed) return;

	FVector location;
	FVector direction;
	FHitResult hitResult;
	bool deprojected = DeprojectMousePositionToWorld(location, direction);
	if (!deprojected) return;

	FVector2D cursorCoordInZPlane = _findCoordinatesInZPlane(_hitLocation.Z, location, direction);
	if (_resizableActor != nullptr)
	{
		auto tableLocation = _resizableActor->GetResizableLocation();
		FVector2D newSize = 2 * (cursorCoordInZPlane - FVector2D(tableLocation.X, tableLocation.Y) ).GetAbs();
		//auto currentSize = FVector2D(_resizableActor->GetSize().X, _resizableActor->GetSize().Y);
		//auto sizeDiff = newSize - currentSize;
		_resizableActor->Resize(FVector(newSize, _resizableActor->GetSize().Z));
	}

}


void AMousePlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay called"));
}


void AMousePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseBtn", IE_Pressed, this, &AMousePlayerController::LeftButtonClicked);
	InputComponent->BindAction("LeftMouseBtn", IE_Released, this, &AMousePlayerController::LeftButtonReleased);

	InputComponent->BindAction("MoveCamera", IE_Pressed, this, &AMousePlayerController::OnMoveCameraPressed);
	InputComponent->BindAction("MoveCamera", IE_Released, this, &AMousePlayerController::OnMoveCameraReleased);
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
	FVector location;
	FVector direction;
	FHitResult hitResult;

	DeprojectMousePositionToWorld(location, direction);
	FCollisionQueryParams queryParams(FName(TEXT("CornerTrace")), false, nullptr);
	bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, location, (location + 1000 * direction), ECC_Visibility, queryParams);
	if (hit)
		PrintDebugMessage(TEXT("hit"));
	else
		PrintDebugMessage(TEXT("no hit"));

	auto hitActor = Cast<IResizable>(hitResult.Actor);
	if (!hitActor) {
		PrintDebugMessage("no actor");
		return;
	}

	_resizableActor = hitActor;
	_hitLocation = hitResult.ImpactPoint;

	bCornerPressed = true;
}

void AMousePlayerController::LeftButtonReleased()
{
	bCornerPressed = false;
}

FVector2D AMousePlayerController::_findCoordinatesInZPlane(float Zvalue, FVector location, FVector direction)
{
	FVector2D result{};
	float zRatio = (Zvalue - location.Z);
	if (direction.Z != 0) {
		zRatio /= direction.Z;
	}
	result.X = location.X+direction.X* zRatio;
	result.Y = location.Y+direction.Y* zRatio;

	return(result);
}