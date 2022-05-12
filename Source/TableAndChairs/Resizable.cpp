// Fill out your copyright notice in the Description page of Project Settings.


#include "Resizable.h"

// Add default functionality here for any IResizable functions that are not pure virtual.

const FVector& IResizable::GetSize()
{
	return _size;
}

void IResizable::SetSize(const FVector& size)
{
	_size = size;
}

void IResizable::Resize(const FVector& newSize)
{
}

void IResizable::Resize(float newX, float newY)
{
}

FVector IResizable::GetResizableLocation()
{
	return FVector();
}