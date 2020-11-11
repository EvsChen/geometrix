// Fill out your copyright notice in the Description page of Project Settings.


#include "Global.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGlobal::AGlobal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USoundCue> pickUpSound(TEXT("/Game/SideScrollerCPP/Audio/PickUpSound.PickUpSound"));
	if (pickUpSound.Object != NULL)
	{
		pickUpSoundCue = (USoundCue*)pickUpSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundCue> dropSound(TEXT("/Game/SideScrollerCPP/Audio/DropSound.DropSound"));
	if (dropSound.Object != NULL)
	{
		dropSoundCue = (USoundCue*)dropSound.Object;
	}
}

// Called when the game starts or when spawned
void AGlobal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGlobal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGlobal::addScore() {
	Score += 1;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), pickUpSoundCue, GetActorLocation());
}

void AGlobal::reduceScore() {
	
	if (Score > 0) {
		Score -= 1;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), dropSoundCue, GetActorLocation());
		
	}
	if (Score < 0) {
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/SideScrollerCPP/Maps/LoseLevel"), TRAVEL_Absolute);
	}
}
