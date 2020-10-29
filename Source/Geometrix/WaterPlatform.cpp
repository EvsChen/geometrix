// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterPlatform.h"
#include "GeometrixCharacter.h"
#include "MyPawn.h"

// Sets default values
AWaterPlatform::AWaterPlatform()
{
	static ConstructorHelpers::FObjectFinder<UMaterial>FoundMaterial(TEXT("/Game/WaterMaterial.WaterMaterial"));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BuffBoxComponent =
		CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BuffBoxComponent;
	BuffBoxComponent->InitBoxExtent(FVector(12.0f, 15.0f, 12.0f));
	BuffBoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BuffBoxComponent->SetSimulatePhysics(false);
	BuffBoxComponent->SetEnableGravity(false);
	BuffBoxComponent->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void AWaterPlatform::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &AWaterPlatform::onHit);



}

// Called every frame
void AWaterPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


void AWaterPlatform::onHit(AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit) {
	if (OtherActor && (OtherActor != this) && OtherActor->IsA(AMyPawn::StaticClass()))
	{
        AMyPawn *pawn = Cast<AMyPawn>(OtherActor);
		if (pawn->mGetMaterial() == MaterialEnum::FOAM) {
            pawn->mSetMaterial(MaterialEnum::FOAM_IN_WATER);
		}
	}

}

