// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickBuff.h"
#include "GeometrixCharacter.h"

// Sets default values
ABrickBuff::ABrickBuff()
{
	static ConstructorHelpers::FObjectFinder<UMaterial>FoundMaterial(TEXT("/Game/M_Brick_Cut_Stone.M_Brick_Cut_Stone"));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BuffBoxComponent =
		CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BuffBoxComponent;
	BuffBoxComponent->InitBoxExtent(FVector(12.0f, 15.0f, 12.0f));
	BuffBoxComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BuffBoxComponent->SetSimulatePhysics(true);
	BuffBoxComponent->SetEnableGravity(false);
	BuffBoxComponent->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void ABrickBuff::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ABrickBuff::onHit);



}

// Called every frame
void ABrickBuff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABrickBuff::onHit(AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit) {
	if (OtherActor && (OtherActor != this) && OtherActor->IsA(APawn::StaticClass()))
	{
		Destroy();
		CurrentShape = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		CurrentShape->SetMassScale(NAME_None, 100000.0f);
		UMaterial* FoundMaterial = LoadObject<UMaterial>(NULL, TEXT("/Game/M_Brick_Cut_Stone.M_Brick_Cut_Stone"), NULL, LOAD_None, NULL);
		StoredMaterial = FoundMaterial;
		DynamicMaterialInst = UMaterialInstanceDynamic::Create(StoredMaterial, CurrentShape);
		CurrentShape->SetMaterial(0, DynamicMaterialInst);
		CurrentShape->RecreatePhysicsState();


	}

}

