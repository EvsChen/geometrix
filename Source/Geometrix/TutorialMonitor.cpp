// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialMonitor.h"

// Sets default values
ATutorialMonitor::ATutorialMonitor() :
    TutorialStr("Init str")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATutorialMonitor::BeginPlay()
{
	Super::BeginPlay();
  TutorialStr = "Press 1-3 to change the shape";
}

void ATutorialMonitor::RecordShapeChange() {
  if (!sphereChanged) {
    sphereChanged = true;
    TutorialStr = "Press RightArrow/D to move right, LeftArrow/A to move left";
  }
}

void ATutorialMonitor::RecordMovement(float val) {
  if (moveRight && moveLeft) {
    return;
  }
  if (val > 0) {
    moveRight = true;
  } else if (val < 0) {
    moveLeft = true;
  }
  if (moveRight && moveLeft) {
    TutorialStr = "Try to climb the stair using the right geometry!";
  }
}

// Called every frame
void ATutorialMonitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

