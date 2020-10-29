// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTriggerBoxFire.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

// include draw debu helpers header file

#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "MyPawn.h"

#include "Kismet/GameplayStatics.h"

AMyTriggerBoxFire::AMyTriggerBoxFire()
{
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBoxFire::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AMyTriggerBoxFire::OnOverlapEnd);
    
}

// Called when the game starts or when spawned
void AMyTriggerBoxFire::BeginPlay()
{
    Super::BeginPlay();

    //DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
    
}

void AMyTriggerBoxFire::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    // check if Actors do not equal nullptr and that
    if (OtherActor && (OtherActor != this)) {
        // print to screen using above defined method when actor enters trigger box
        AMyPawn* geo = Cast<AMyPawn>(OtherActor);
        if (geo != nullptr) {
            if (geo->shape == ShapeEnum::CROSS) {
                print("Safe");
            }
            else {
                print("Hurt! It is not a cross");
            }
        }
        else {
            print("It is not a geometry");
        }
        
        
        //printFString("OtherActor Actor = %s", *OtherActor->GetName());
    }
}

void AMyTriggerBoxFire::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
    if (OtherActor && (OtherActor != this)/* && OtherActor is Sphere*/) {
        // print to screen using above defined method when actor leaves trigger box
        //print("Close door");
        //printFString("%s has left the Trigger Box", *OtherActor->GetName());

    }
}
