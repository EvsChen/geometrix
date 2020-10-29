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
#include "Math/Box.h"
#include <vector>


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
    
    int cubeScaleZ = 2;
    FBox cubeBox = CubeMesh->GetBoundingBox();
    FVector cubeMin = cubeBox.Min, cubeMax = cubeBox.Max;
    cubeMin[2] *= cubeScaleZ;
    cubeMax[2] *= cubeScaleZ;
    float cubeX = (cubeMin[0] + cubeMax[0]) / 2.f;
    // Left bottom
    cubeBoundLocal[0] = FVector(cubeX, cubeMax[1], cubeMin[2]);
    // Left top
    cubeBoundLocal[1] = FVector(cubeX, cubeMax[1], cubeMax[2]);
    // Right top
    cubeBoundLocal[2] = FVector(cubeX, cubeMin[1], cubeMax[2]);
    // Right bottom
    cubeBoundLocal[3] = FVector(cubeX, cubeMin[1], cubeMin[2]);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
    SwitchShape(1);
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

bool clamp(float &vel, float max, float min) {
    bool clamped = false;
    if (vel > max) {
        vel = max;
        clamped = true;
    }
    else if (vel < min) {
        vel = min;
        clamped = true;
    }
    return clamped;
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
    FVector angVel = CurrentShape->GetPhysicsAngularVelocityInDegrees();
    FVector linVel = CurrentShape->GetPhysicsLinearVelocity();
//    UE_LOG(LogTemp, Warning, TEXT("angVel is %f %f %f"), angVel[0], angVel[1], angVel[2]);
    UE_LOG(LogTemp, Warning, TEXT("linVel is %f %f %f"), linVel[0], linVel[1], linVel[2]);
    bool exceedAngVel = false, exceedLinVel = false;
    if (shape == ShapeEnum::CUBE) {
        exceedAngVel = exceedAngVel || clamp(angVel[0], 100, -100);
        exceedLinVel = exceedLinVel || clamp(linVel[2], 400, -400);
    } else if (shape == ShapeEnum::SPHERE) {
        exceedAngVel = exceedAngVel || clamp(angVel[0], 500, -500);
        exceedLinVel = exceedLinVel || clamp(linVel[2], 400, -400);
    }
    if (exceedAngVel) {
        CurrentShape->SetPhysicsAngularVelocityInDegrees(angVel);
    }
    if (exceedLinVel) {
        CurrentShape->SetPhysicsLinearVelocity(linVel);
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
      CurrentShape->SetRelativeScale3D(FVector(1, 1, 2));
  } else if (shapeIdx == 2) {
      shape = ShapeEnum::SPHERE;
      CurrentShape->SetStaticMesh(SphereMesh);
      CurrentShape->SetRelativeScale3D(FVector(1, 1, 1));
  } else if (shapeIdx == 3) {
      shape = ShapeEnum::WEDGE;
      CurrentShape->SetStaticMesh(WedgeMesh);
      CurrentShape->SetWorldRotation(FRotator(0, 90, 90));
      CurrentShape->SetRelativeScale3D(FVector(1, 1, 1));
  }
    CurrentShape->RecreatePhysicsState();
//    CurrentShape->SetRelativeLocation_Direct(FVector(0, 0, 0));
}


void AMyPawn::MoveRight(float Value)
{
    if (Value < 0.001 && Value > -0.001) {
        return;
    }
    if (shape == ShapeEnum::CUBE) {
        // Find the contact point with ground
        FVector loc = GetActorLocation();
        const FTransform &trans = GetTransform();
        TArray<FVector> worldBound;
        for (int i = 0; i < 4; i++) {
            FVector &b = cubeBoundLocal[i];
            // Get bound in world space
            worldBound.Push(loc + trans.TransformVector(b));
        }
        FVector center = (worldBound[0] + worldBound[2]) / 2.f;
        // Get the lowest two vertices
        worldBound.Sort([](const FVector &a, const FVector &b) {
            return a[2] < b[2];
        });
        FVector contactRight, contactLeft;
        // -Y is the positive direction
        if (worldBound[0][1] < worldBound[1][1]) {
            contactRight = worldBound[0];
            contactLeft = worldBound[1];
        } else {
            contactRight = worldBound[1];
            contactLeft = worldBound[0];
        }
        bool moveRight = Value > 0;
        FVector contactPoint = moveRight ? contactRight : contactLeft;
        FVector forcePoint = center + (center - contactPoint);
        FVector forceDir = FVector::CrossProduct(FVector(1, 0, 0), forcePoint - center);
        FVector force = forceDir.GetUnsafeNormal() * Value * 3000;
//        UE_LOG(LogTemp, Warning, TEXT("contactPoint is %f %f %f"), contactPoint[0], contactPoint[1], contactPoint[2]);
//        UE_LOG(LogTemp, Warning, TEXT("center is %f %f %f"), center[0], center[1], center[2]);
//        UE_LOG(LogTemp, Warning, TEXT("force is %f %f %f"), force[0], force[1], force[2]);
//        UE_LOG(LogTemp, Warning, TEXT("forcePoint is %f %f %f"), forcePoint[0], forcePoint[1], forcePoint[2]);
        CurrentShape->AddImpulseAtLocation(force, forcePoint, NAME_None);
        
//        FVector axis(0, 0, -1);
//        axis *= Value * 5000;
//        CurrentShape->AddTorqueInDegrees(axis, NAME_None, true);
    } else if (shape == ShapeEnum::SPHERE) {
        Value *= -1000;
        CurrentShape->AddImpulse(FVector(0, Value, 0));
    } else if (shape == ShapeEnum::WEDGE) {
        Value *= -3000;
        CurrentShape->AddImpulse(FVector(0, Value, 0));
    }
    //UE_LOG(LogTemp, Warning, TEXT("Move right with value %f"), Value);
}

