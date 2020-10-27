// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "MyDefaultPawn.generated.h"

UCLASS()
class GEOMETRIX_API AMyDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()
    
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* CurrentShape;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* SideViewCameraComponent;

    /** Camera boom positioning the camera beside the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;
    
    DECLARE_DELEGATE_OneParam(ShapeDelegate, int32);
    
    class UStaticMesh *CubeMesh, *SphereMesh, *WedgeMesh;
    
    void SwitchShape(int shape);

public:
    AMyDefaultPawn();
    
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
