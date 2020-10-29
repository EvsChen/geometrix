// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterPlatform.h"
#include "GeometrixCharacter.h"

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
	if (OtherActor && (OtherActor != this) && OtherActor->IsA(APawn::StaticClass()))
	{
		CurrentShape = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		

		UMaterial* FoundMaterial = LoadObject<UMaterial>(NULL, TEXT("/Game/foamMaterial.foamMaterial"), NULL, LOAD_None, NULL);
		UMaterial* FoundMaterial2 = LoadObject<UMaterial>(NULL, TEXT("/Game/iceMaterial.iceMaterial"), NULL, LOAD_None, NULL);
		UE_LOG(LogTemp, Warning, TEXT("%s %s %s %s"), FoundMaterial, FoundMaterial2,  CurrentShape->GetMaterial(0)->GetMaterial(), CurrentShape->GetMaterial(1));
		if (CurrentShape->GetMaterial(0)->GetMaterial() == FoundMaterial) {
		
			CurrentShape->SetMassScale(NAME_None, 40.0f);
			
			//UMaterialInstanceDynamic* mesh1P = ((UMaterialInstanceDynamic*)(CurrentShape->GetMaterial(0)))->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(0.080128f, 0.0762f, 1.0f));
			UMaterialInterface* MeshMat = CurrentShape->GetMaterial(0);
			UMaterialInstanceDynamic* dy = UMaterialInstanceDynamic::Create(MeshMat, this);
			dy->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(0.080128f, 0.0762f, 1.0f));
			CurrentShape->SetMaterial(0, dy);
			UE_LOG(LogTemp, Warning, TEXT("mass"));
		}


	}

}

