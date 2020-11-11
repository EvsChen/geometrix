// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "GeometrixGameMode.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Global.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    OnActorBeginOverlap.AddDynamic(this, &ACheckpoint::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpoint::OnBeginOverlap(AActor *thisAtor, AActor *otherActor) {
    UWorld *world = GetWorld();
    AGeometrixGameMode *GameMode = (AGeometrixGameMode *)world->GetAuthGameMode();
    UMyGameInstance *GI = (UMyGameInstance *)UGameplayStatics::GetGameInstance(world);
    AGlobal *glob = (AGlobal *)UGameplayStatics::GetActorOfClass(world, AGlobal::StaticClass());
    
    FString currentLevel = UGameplayStatics::GetCurrentLevelName(world);
    FVector curPos = GetActorLocation();
    GI->loseStartPos.Add(currentLevel, curPos);
    GI->coinsWhenLost.Add(currentLevel, glob->Score);
    GameMode->gameStatusString = "Current progress saved";
    Destroy();
}
