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
#include "Materials/Material.h"

#define EPS 0.001

void debugVec(FString s, const FVector &v) {
    UE_LOG(LogTemp, Warning, TEXT("%s is %f %f %f"), *s, v[0], v[1], v[2]);
}

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
    
    FoamMat = LoadObject<UMaterial>(NULL, TEXT("/Game/foamMaterial.foamMaterial"), NULL, LOAD_None, NULL);
    SetCubeBounds();
    SetWedgeBounds();

    //coin collection
    //coinNum = 0;
}

void AMyPawn::SetCubeBounds() {
    int cubeScaleZ = 2;
    FBox cubeBox = CubeMesh->GetBoundingBox();
    FVector cubeMin = cubeBox.Min, cubeMax = cubeBox.Max;
    cubeMin[2] *= cubeScaleZ;
    cubeMax[2] *= cubeScaleZ;
    // Left bottom
    cubeBoundLocal[0] = FVector(0, cubeMax[1], cubeMin[2]);
    // Left top
    cubeBoundLocal[1] = FVector(0, cubeMax[1], cubeMax[2]);
    // Right top
    cubeBoundLocal[2] = FVector(0, cubeMin[1], cubeMax[2]);
    // Right bottom
    cubeBoundLocal[3] = FVector(0, cubeMin[1], cubeMin[2]);
}

void AMyPawn::SetWedgeBounds() {
    // right most
    wedgeBoundLocal[0] = FVector(0, 50, 50);
    // left top
    wedgeBoundLocal[1] = FVector(50, -50, 50);
    // left bottom
    wedgeBoundLocal[2] = FVector(-50, -50, 50);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
    SwitchShape(1);
    // Setup camera following
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
    
//    OnActorHit.AddDynamic(this, &AMyPawn::onHit);
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
    AdjustCameraPos();
    FVector angVel = CurrentShape->GetPhysicsAngularVelocityInDegrees();
    FVector linVel = CurrentShape->GetPhysicsLinearVelocity();
    bool exceedAngVel = false, exceedLinVel = false;
    if (shape == ShapeEnum::CUBE) {
        exceedAngVel = exceedAngVel || clamp(angVel[0], 100, -100);
        exceedLinVel = exceedLinVel || clamp(linVel[2], 400, -400);
    } else if (shape == ShapeEnum::SPHERE) {
        exceedAngVel = exceedAngVel || clamp(angVel[0], 500, -500);
        exceedLinVel = exceedLinVel || clamp(linVel[2], 400, -400);
    } else if (shape == ShapeEnum::WEDGE) {
        exceedAngVel = exceedAngVel || clamp(angVel[0], 60, -60);
        exceedLinVel = exceedLinVel || clamp(linVel[2], 300, -300);
    }
    if (exceedAngVel) {
        CurrentShape->SetPhysicsAngularVelocityInDegrees(angVel);
    }
    if (exceedLinVel) {
        CurrentShape->SetPhysicsLinearVelocity(linVel);
    }

    //Die if fall
    FVector loc = GetActorLocation();
    if (loc[2] < -2000.f) {

    }
}

void AMyPawn::AdjustCameraPos() {
    if (!SideViewCamera) {
        return;
    }
    FVector curLoc = GetActorLocation();
    FVector cameraLoc = SideViewCamera->GetActorLocation();
    // Match horizontal position
    cameraLoc[1] = curLoc[1];
    float diffZ = curLoc[2] - cameraLoc[2];
    if (diffZ < -100 || diffZ > 100) {
        float speedUp = (abs(diffZ) - 100.f) / 100.f;
        speedUp = speedUp * speedUp;
        float speed = speedUp * 3;
        cameraLoc[2] += diffZ > 0 ? speed : -speed;
    }
    SideViewCamera->SetActorLocation(cameraLoc);
    
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);
    PlayerInputComponent->BindAxis("TranslateRight", this, &AMyPawn::TranslateRight);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape1", IE_Pressed, this, &AMyPawn::SwitchShape, 1);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape2", IE_Pressed, this, &AMyPawn::SwitchShape, 2);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape3", IE_Pressed, this, &AMyPawn::SwitchShape, 3);
}

void AMyPawn::SwitchShape(int shapeIdx) {
  if (shapeIdx == 1) {
      shape = ShapeEnum::CUBE;
      CurrentShape->SetStaticMesh(CubeMesh);
      CurrentShape->SetRelativeRotation(FRotator(0, 0, 0));
      CurrentShape->SetRelativeScale3D(FVector(1, 1, 2));
  } else if (shapeIdx == 2) {
      shape = ShapeEnum::SPHERE;
      CurrentShape->SetStaticMesh(SphereMesh);
      CurrentShape->SetRelativeRotation(FRotator(0, 0, 0));
      CurrentShape->SetRelativeScale3D(FVector(1, 1, 1));
  } else if (shapeIdx == 3) {
      shape = ShapeEnum::WEDGE;
      CurrentShape->SetStaticMesh(WedgeMesh);
      CurrentShape->SetRelativeRotation(FRotator(90, 0, 0));
      CurrentShape->AddWorldOffset(FVector(0, 0, 20));
      CurrentShape->SetRelativeScale3D(FVector(1, 1, 1));
  }
    CurrentShape->RecreatePhysicsState();
//    CurrentShape->SetRelativeLocation_Direct(FVector(0, 0, 0));
}


void AMyPawn::TranslateRight(float Value) {
    if (Value < 0.001 && Value > -0.001) {
        return;
    }
    CurrentShape->AddImpulse(FVector(0, Value * -100, 0));
}


void AMyPawn::MoveRight(float Value)
{
    if (Value < 0.001 && Value > -0.001) {
        return;
    }
    float massScale = CurrentShape->GetMassScale();
    Value *= massScale;
    if (shape == ShapeEnum::CUBE) {
        FVector force, forcePoint;
        GetCubeForce(Value, force, forcePoint);
        CurrentShape->AddImpulseAtLocation(force, forcePoint, NAME_None);
        
    } else if (shape == ShapeEnum::SPHERE) {
        Value *= -1000;
        CurrentShape->AddImpulse(FVector(0, Value, 0));
    } else if (shape == ShapeEnum::WEDGE) {
        FVector force, forcePoint;
        GetWedgeForce(Value, force, forcePoint);
        CurrentShape->AddImpulseAtLocation(force, forcePoint, NAME_None);
    }
    //UE_LOG(LogTemp, Warning, TEXT("Move right with value %f"), Value);
}

void AMyPawn::mSetMaterial(MaterialEnum mat) {
    m_Mat = mat;
    if (mat == MaterialEnum::FOAM) {
        CurrentShape->SetMassScale(NAME_None,0.1f);
        UMaterialInstanceDynamic *DynamicMaterialInst = UMaterialInstanceDynamic::Create(FoamMat, CurrentShape);
        CurrentShape->SetMaterial(0, DynamicMaterialInst);
    } else if (mat == MaterialEnum::FOAM_IN_WATER) {
        CurrentShape->SetMassScale(NAME_None, 4.0f);
        UMaterialInterface* MeshMat = CurrentShape->GetMaterial(0);
        UMaterialInstanceDynamic* dy = UMaterialInstanceDynamic::Create(MeshMat, this);
        dy->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(0.080128f, 0.0762f, 1.0f));
        CurrentShape->SetMaterial(0, dy);
    }
}

MaterialEnum AMyPawn::mGetMaterial() {
    return m_Mat;
}

void AMyPawn::GetCubeForce(float Value, FVector &force, FVector &forcePoint) const {
    // Find the contact point with ground
    FVector loc = GetActorLocation();
    const FTransform &trans = GetTransform();
    FVector localCenter(0, 0, 100);
    FVector center = loc + trans.TransformVector(localCenter);
    FVector contactPoint;
    TArray<FVector> worldBound;
    for (int i = 0; i < 4; i++) {
        FVector b = cubeBoundLocal[i];
        // Get bound in world space
        worldBound.Push(loc + trans.TransformVector(b));
    }
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
    contactPoint = moveRight ? contactRight : contactLeft;
    forcePoint = center + (center - contactPoint);
    FVector forceDir = FVector::CrossProduct(FVector(1, 0, 0), forcePoint - center);
    force = forceDir.GetUnsafeNormal() * Value * 3000;
}


void AMyPawn::GetWedgeForce(float Value, FVector &force, FVector &forcePoint) const {
    // Find the contact point with ground
    bool moveRight = Value > 0;
    FVector loc = GetActorLocation();
    const FTransform &trans = GetTransform();
    using boundP = std::pair<FVector, int>;
    TArray<boundP> worldBound;
    for (int i = 0; i < 3; i++) {
        FVector b = wedgeBoundLocal[i];
        // Get bound in world space
        worldBound.Push({ loc + trans.TransformVector(b), i });
    }
    FVector center = loc + trans.TransformVector(FVector(0, 0, 50));
    // Get the lowest two vertices
    worldBound.Sort([](const boundP &a, const boundP &b) {
        return a.first[2] < b.first[2];
    });
    int idx;
    float lowz1 = worldBound[0].first[2],
          lowz2 = worldBound[1].first[2];
    if (lowz2 - lowz1 > EPS) {
        idx = 0;
    } else {
        int rightIdx, leftIdx;
        // -Y is the positive direction
        if (worldBound[0].first[1] < worldBound[1].first[1]) {
            rightIdx = 0;
            leftIdx = 1;
        } else {
            rightIdx = 1;
            leftIdx = 0;
        }
        idx = moveRight ? rightIdx : leftIdx;
    }
    FVector contactPoint = worldBound[idx].first;
    int vertIdx = worldBound[idx].second;
    int forceVertIdx = moveRight ? (vertIdx + 1) % 3 : (vertIdx + 2) % 3;
    for (int i = 0; i < 3; i++) {
        if (worldBound[i].second == forceVertIdx) {
            forcePoint = worldBound[i].first;
            break;
        }
    }
    FVector forceDir = FVector::CrossProduct(FVector(1, 0, 0), forcePoint - center);
    force = forceDir.GetUnsafeNormal() * Value * 1500;
}


void AMyPawn::onHit(AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
    if (!OtherActor || OtherActor == this) {
        return;
    }
    FString name = OtherActor->GetName();
    if (OtherActor->ActorHasTag(TEXT("collideSolid"))) {
        collidedSolid = OtherActor;
        FVector_NetQuantize res = Hit.ImpactPoint;
        for (int i = 0; i < 3; i++) hitPoint[i] = res[i];
    }
}
