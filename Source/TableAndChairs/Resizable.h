// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Resizable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UResizable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TABLEANDCHAIRS_API IResizable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetResizableLocation();
	virtual const FVector& GetSize();
	virtual void SetSize(const FVector& size);
	virtual void Resize(const FVector& newSize);
	virtual void Resize(float newX, float newY);

private:
	FVector _size;
};
