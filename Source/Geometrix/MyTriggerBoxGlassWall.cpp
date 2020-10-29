// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTriggerBoxGlassWall.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

// include draw debu helpers header file
#include "MyTriggerBoxGlassWall.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "MyPawn.h"

#include "Kismet/GameplayStatics.h"

AMyTriggerBoxGlassWall::AMyTriggerBoxGlassWall()
{
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBoxGlassWall::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AMyTriggerBoxGlassWall::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AMyTriggerBoxGlassWall::BeginPlay()
{
    Super::BeginPlay();

    //DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);

}

void AMyTriggerBoxGlassWall::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    // check if Actors do not equal nullptr and that
    if (OtherActor && (OtherActor != this)) {
        // print to screen using above defined method when actor enters trigger box
        AMyPawn* geo = Cast<AMyPawn>(OtherActor);
        if (geo != nullptr) {
            if (geo->shape == ShapeEnum::SPHERE) {
                print("Open door");
                //
                TArray<AActor*> Actors;
                UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("glass"), Actors);
     
                print("Found");
                for (AActor* Actor : Actors)
                {

                    Actor->SetActorEnableCollision(false);
                    print("Disable Collision");

                }
            }
            else {
                print("Cannot go through! It is not a sphere");
            }
        }
        else {
            print("It is not a geometry");
        }


        //printFString("OtherActor Actor = %s", *OtherActor->GetName());
    }
}

void AMyTriggerBoxGlassWall::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
    if (OtherActor && (OtherActor != this)/* && OtherActor is Sphere*/) {
        // print to screen using above defined method when actor leaves trigger box
        //print("Close door");
        printFString("%s has left the Trigger Box", *OtherActor->GetName());
        TArray<AActor*> Actors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("glass"), Actors);
        for (AActor* Actor : Actors)
        {

            Actor->SetActorEnableCollision(true);

        }
    }
}
