#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Char.generated.h"

UCLASS(config = Game)
class AMortarCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Base turn rate, in deg/sec */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tweak_Look)
	float BaseTurnRate = 45.f;

	/** Base look up/down rate, in deg/sec */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tweak_Look)
	float BaseLookUpRate = 45.f;

	/** Run speed multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tweak_Move)
	float RunModifier = 2.f;

	/** Running or walking */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tweak_Move)
	bool bIsRunning = false;

public:
	AMortarCharacter();

	void MoveForward(float); // Handles moving forward/backward
	void MoveRight(float); // Handles strafing movement, left and right
	void TurnAtRate(float); // rate is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void LookUpAtRate(float); // rate is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void StartRunning();
	void StopRunning();

protected:
	virtual void BeginPlay() override;
};
