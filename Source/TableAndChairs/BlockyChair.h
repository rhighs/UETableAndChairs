#pragma once

#include "CoreMinimal.h"
#include "Mesh.h"

/** Generate a chair using 6 cuboid meshes (4 legs a chair base and a chair back rest) 
* \param the table size
* \return the chair mesh buffers
*/
Mesh BlockyChair(const FVector& size);
