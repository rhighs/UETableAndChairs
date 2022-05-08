#pragma once

#include "CoreMinimal.h"
#include "Mesh.h"

/** Generate a table using 5 cuboid meshes (4 legs and 1 table top) 
* \param the table size
* \return the table mesh buffers
*/
Mesh BlockyTable(const FVector& size);

/** Greedely generate spawn locations for chairs around a rectangluar table
* \param the table size
* \param the chairs size
* \return a position and the rotation vector in deg for each chair
*/
TArray<TTuple<FVector, FVector>> ChairPositions(const FVector& tableSize, const FVector& chairSize);

