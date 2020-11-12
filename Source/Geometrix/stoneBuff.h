// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "Materials/Material.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "stoneBuff.generated.h"

UCLASS()
class GEOMETRIX_API AstoneBuff : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AstoneBuff();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UBoxComponent* BuffBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* CurrentShape;

	// Projectile movement component
	UFUNCTION()
		void onHit(AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere)
		UMaterialInstanceDynamic* DynamicMaterialInst;

	UPROPERTY(VisibleAnywhere)
		UMaterial* StoredMaterial;
};
