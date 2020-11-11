// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GeometrixGameMode.generated.h"

UCLASS(minimalapi)
class AGeometrixGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGeometrixGameMode();
  TMap<FString, FVector> defaultStartPos;
  TMap<FString, FVector> loseStartPos; 
};



