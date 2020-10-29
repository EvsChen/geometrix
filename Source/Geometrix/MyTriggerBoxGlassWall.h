// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBoxGlassWall.generated.h"

/**
 *
 */
UCLASS()
class GEOMETRIX_API AMyTriggerBoxGlassWall : public ATriggerBox
{
    GENERATED_BODY()

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    // constructor sets default values for this actor's properties
    AMyTriggerBoxGlassWall();

    // declare overlap begin function
    UFUNCTION()
        void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

    // declare overlap end function
    UFUNCTION()
        void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

};
