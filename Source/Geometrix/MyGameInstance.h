// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GEOMETRIX_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    UMyGameInstance();
    TMap<FString, FVector> defaultStartPos;
    TMap<FString, FVector> loseStartPos;
    TMap<FString, int> coinsWhenLost;
};
