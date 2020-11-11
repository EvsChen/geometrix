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
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString gameStatusString;

    float statusTime = 0.f;
    
    virtual void Tick(float DeltaTime) override;
};



