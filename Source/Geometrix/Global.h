// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SoundDefinitions.h"
#include "Sound/SoundCue.h"

#include "Global.generated.h"

UCLASS()
class GEOMETRIX_API AGlobal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlobal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void addScore();
	UFUNCTION(BlueprintCallable)
	void reduceScore();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global")
	int Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global")
	int maxScore;

	UPROPERTY(VisibleAnywhere)
	USoundCue* pickUpSoundCue;

	UPROPERTY(VisibleAnywhere)
	USoundCue* dropSoundCue;
};
