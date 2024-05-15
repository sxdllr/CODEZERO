
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComponentPicker.generated.h"

class ABaseGun;
class ABaseCharacter;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, ABaseCharacter*, PickUpCharacter);
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CODEZERO_API AComponentPicker : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	USphereComponent* PickUpCollision;
	
public:
	AComponentPicker();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PickUpGunRadius(ABaseCharacter* Character);

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUp OnPickUp;

	UFUNCTION(BlueprintCallable, Category = "Collision")
	USphereComponent* GetPickUpCollision() const { return PickUpCollision; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:  
	ABaseGun* FindGunToPickUp();

	UPROPERTY()
	ABaseGun* FoundGun;

	bool bIsOverlap;

	float PickUpRadius;
};