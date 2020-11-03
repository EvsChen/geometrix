// Fill out your copyright notice in the Description page of Project Settings.


#include "GlassPlatform.h"
#include "GeometrixCharacter.h"

// Sets default values
AGlassPlatform::AGlassPlatform()
{
	static ConstructorHelpers::FObjectFinder<UMaterial>FoundMaterial(TEXT("/Game/WaterMaterial.WaterMaterial"));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BuffBoxComponent =
		CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BuffBoxComponent;
	BuffBoxComponent->InitBoxExtent(FVector(12.0f, 15.0f, 12.0f));
	BuffBoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
	BuffBoxComponent->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void AGlassPlatform::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &AGlassPlatform::onHit);



}

// Called every frame
void AGlassPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


void AGlassPlatform::onHit(AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit) {
	if (OtherActor && (OtherActor != this) && OtherActor->IsA(APawn::StaticClass()))
	{
		BuffBoxComponent =
			CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
		BuffBoxComponent->SetSimulatePhysics(true);
		BuffBoxComponent->SetEnableGravity(true);
		CurrentShape = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));


	}

}

