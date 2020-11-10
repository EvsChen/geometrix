// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

enum class ShapeEnum { CUBE, SPHERE, WEDGE, CROSS };
enum class MaterialEnum { DEFAULT, ICE, FOAM, FOAM_IN_WATER };

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
    class UMaterial *FoamMat;
    
    FVector cubeBoundLocal[4];
    FVector wedgeBoundLocal[3];
    
    // Useful template for adding parameters to bindAction
    DECLARE_DELEGATE_OneParam(ShapeDelegate, int32);

public:
	  // Sets default values for this pawn's properties
	  AMyPawn();
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    ShapeEnum shape;
    
    void mSetMaterial(MaterialEnum mat);
    MaterialEnum mGetMaterial();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    void SwitchShape(int shape);
    
    void MoveRight(float Value);
    
    void TranslateRight(float Value);
    
    // Return the force to rotate for cube
    void GetCubeForce(float Value, FVector &force, FVector &forcePoint) const;
    
    // Return the force to rotate for cube
    void GetWedgeForce(float Value, FVector &force, FVector &forcePoint) const;
    
    UFUNCTION()
    void onHit(AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
    
    AActor *collidedSolid;
    AActor *tutMonitor;
    FVector hitPoint;
    MaterialEnum m_Mat;
    
private:
    void SetCubeBounds();
    void SetWedgeBounds();
    void AdjustCameraPos();

    //coin collection
    //int coinNum;
};
