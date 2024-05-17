
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/BaseCharacter.h"
#include "ComponentPicker.generated.h"

class ABaseGun;
class USphereComponent;
class IInteractionInterface;

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
	void PickUpItemRadius(ABaseCharacter* Character);

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUp OnPickUp;

	UFUNCTION(BlueprintCallable, Category = "Collision")
	USphereComponent* GetPickUpCollision() const { return PickUpCollision; }

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void InteractionCheck(const ACharacter* OtherActor);

	void BeingOverlapped(AActor* NewInteractable);
	void FoundInteractable(AActor* NewInteractable);

	TScriptInterface<IInteractionInterface> TargetInteractable;

	FInteractionData InteractionData;

private:  
	ABaseGun* FindItemToPickUp() const;

	UPROPERTY()
	ABaseGun* FoundGun;

	UPROPERTY()
	ABaseCharacter* OverlappedCharacter;

	bool bIsOverlap;

	float PickUpRadius;
};