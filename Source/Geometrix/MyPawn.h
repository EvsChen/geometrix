// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

enum class ShapeEnum { CUBE, SPHERE, WEDGE, CROSS };

UCLASS()
class GEOMETRIX_API AMyPawn : public APawn
{
	GENERATED_BODY()
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USceneComponent* DummyComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* SideViewCameraComponent;

    /** Camera boom positioning the camera beside the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* CurrentShape;
    
    class ACameraActor *SideViewCamera;
  
    class UStaticMesh *CubeMesh, *SphereMesh, *WedgeMesh;
    
    FVector cubeBoundLocal[4];
    
    // Useful template for adding parameters to bindAction
    DECLARE_DELEGATE_OneParam(ShapeDelegate, int32);

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    void SwitchShape(int shape);
    
    void MoveRight(float Value);
    
    ShapeEnum shape;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
