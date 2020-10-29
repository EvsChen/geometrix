#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBoxFire.generated.h"

/**
 *
 */
UCLASS()
class GEOMETRIX_API AMyTriggerBoxFire : public ATriggerBox
{
    GENERATED_BODY()
    
protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    // constructor sets default values for this actor's properties
    AMyTriggerBoxFire();

    // declare overlap begin function
    UFUNCTION()
        void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

    // declare overlap end function
    UFUNCTION()
        void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
    
};
