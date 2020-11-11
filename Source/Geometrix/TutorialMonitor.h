// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialMonitor.generated.h"

UCLASS()
class GEOMETRIX_API ATutorialMonitor : public AActor
{
	GENERATED_BODY()
	
public:	
	  // Sets default values for this actor's properties
	  ATutorialMonitor();
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FString TutorialStr;
    
    void RecordShapeChange();
    void RecordMovement(float Val);

    DECLARE_DELEGATE_OneParam(ShapeDelegate, int32);
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
  bool sphereChanged = false,
       moveRight = false,
       moveLeft = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
