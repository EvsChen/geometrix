// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeometrixCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

AGeometrixCharacter::AGeometrixCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
    
    CurrentShape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CurrentShape"));
    ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    ConstructorHelpers::FObjectFinder<UStaticMesh> SphereObj(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    ConstructorHelpers::FObjectFinder<UStaticMesh> WedgeObj(TEXT("/Game/StarterContent/Shapes/Shape_Wedge_A.Shape_Wedge_A"));
    CubeMesh = CubeObj.Object;
    SphereMesh = SphereObj.Object;
    WedgeMesh = WedgeObj.Object;
    SwitchShape(1);
    
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGeometrixCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGeometrixCharacter::MoveRight); 
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape1", IE_Pressed, this, &AGeometrixCharacter::SwitchShape, 1);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape2", IE_Pressed, this, &AGeometrixCharacter::SwitchShape, 2);
    PlayerInputComponent->BindAction<ShapeDelegate>("Shape3", IE_Pressed, this, &AGeometrixCharacter::SwitchShape, 3);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGeometrixCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGeometrixCharacter::TouchStopped);
}

void AGeometrixCharacter::SwitchShape(int shape) {
  if (shape == 1) {
      CurrentShape->SetStaticMesh(CubeMesh);
  } else if (shape == 2) {
      CurrentShape->SetStaticMesh(SphereMesh);
  } else if (shape == 3) {
      CurrentShape->SetStaticMesh(WedgeMesh);
  }
  CurrentShape->SetRelativeLocation_Direct(FVector(0, 0, 0));
}

void AGeometrixCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void AGeometrixCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AGeometrixCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

