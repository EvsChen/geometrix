// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDefaultPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AMyDefaultPawn::AMyDefaultPawn() {
    AutoPossessPlayer = EAutoReceiveInput::Player0;
    
    CurrentShape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CurrentShape"));
    RootComponent = CurrentShape;
    CurrentShape->SetSimulatePhysics(true);
    
    ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    ConstructorHelpers::FObjectFinder<UStaticMesh> SphereObj(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    ConstructorHelpers::FObjectFinder<UStaticMesh> WedgeObj(TEXT("/Game/StarterContent/Shapes/Shape_Wedge_A.Shape_Wedge_A"));
    CubeMesh = CubeObj.Object;
    SphereMesh = SphereObj.Object;
    WedgeMesh = WedgeObj.Object;
    SwitchShape(1);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(CurrentShape);
    CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->TargetArmLength = 500.f;
    CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
    CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

    // Create a camera and attach to boom
    SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
}

void AMyDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyDefaultPawn::MoveRight);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape1", IE_Pressed, this, &AMyDefaultPawn::SwitchShape, 1);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape2", IE_Pressed, this, &AMyDefaultPawn::SwitchShape, 2);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape3", IE_Pressed, this, &AMyDefaultPawn::SwitchShape, 3);
}

void AMyDefaultPawn::SwitchShape(int shape) {
  if (shape == 1) {
      CurrentShape->SetStaticMesh(CubeMesh);
  } else if (shape == 2) {
      CurrentShape->SetStaticMesh(SphereMesh);
  } else if (shape == 3) {
      CurrentShape->SetStaticMesh(WedgeMesh);
  }
  CurrentShape->SetRelativeLocation_Direct(FVector(0, 0, 0));
}
