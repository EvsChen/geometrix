// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"


// Sets default values
AMyPawn::AMyPawn() : shape(ShapeEnum::CUBE)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    AutoPossessPlayer = EAutoReceiveInput::Player0;
    
    CurrentShape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CurrentShape"));
    // This seems to be only working in BluePrint, should check the editor 
    CurrentShape->BodyInstance.bLockXRotation = true;
    CurrentShape->BodyInstance.bLockZRotation = true;
    CurrentShape->BodyInstance.bLockYTranslation = true;
    CurrentShape->SetSimulatePhysics(true);
    RootComponent = CurrentShape;
    
    ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    ConstructorHelpers::FObjectFinder<UStaticMesh> SphereObj(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    ConstructorHelpers::FObjectFinder<UStaticMesh> WedgeObj(TEXT("/Game/StarterContent/Shapes/Shape_Wedge_A.Shape_Wedge_A"));
    CubeMesh = CubeObj.Object;
    SphereMesh = SphereObj.Object;
    WedgeMesh = WedgeObj.Object;
    SwitchShape(1);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
    TArray<AActor*> outActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), outActors);
    for (int i = 0; i < outActors.Num(); i++) {
        FString name = outActors[i]->GetName();
        if (name == "SideViewCamera") {
            SideViewCamera = dynamic_cast<ACameraActor*>(outActors[i]);
        }
    }
    if (!SideViewCamera) {
        UE_LOG(LogTemp, Warning, TEXT("Side view camera not found"));
    } else {
        APlayerController *pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        pc->SetViewTargetWithBlend(SideViewCamera);
    }
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (SideViewCamera) {
        FVector curLoc = GetActorLocation();
        FVector cameraLoc = SideViewCamera->GetActorLocation();
        cameraLoc[1] = curLoc[1];
        SideViewCamera->SetActorLocation(cameraLoc);
    }
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape1", IE_Pressed, this, &AMyPawn::SwitchShape, 1);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape2", IE_Pressed, this, &AMyPawn::SwitchShape, 2);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape3", IE_Pressed, this, &AMyPawn::SwitchShape, 3);
}

void AMyPawn::SwitchShape(int shapeIdx) {
  if (shapeIdx == 1) {
      shape = ShapeEnum::CUBE;
      CurrentShape->SetStaticMesh(CubeMesh);
  } else if (shapeIdx == 2) {
      shape = ShapeEnum::SPHERE;
      CurrentShape->SetStaticMesh(SphereMesh);
  } else if (shapeIdx == 3) {
      shape = ShapeEnum::WEDGE;
      CurrentShape->SetStaticMesh(WedgeMesh);
  }
  CurrentShape->SetRelativeLocation_Direct(FVector(0, 0, 0));
}

void AMyPawn::MoveRight(float Value)
{
    if (Value < 0.001 && Value > -0.001) {
        return;
    }
    if (shape == ShapeEnum::CUBE) {
        Value *= -3000;
        CurrentShape->AddImpulse(FVector(0, Value, 0));
    } else if (shape == ShapeEnum::SPHERE) {
        Value *= -1000;
        CurrentShape->AddImpulse(FVector(0, Value, 0));
    } else if (shape == ShapeEnum::WEDGE) {
        Value *= -3000;
        CurrentShape->AddImpulse(FVector(0, Value, 0));
    }
    //UE_LOG(LogTemp, Warning, TEXT("Move right with value %f"), Value);
}

