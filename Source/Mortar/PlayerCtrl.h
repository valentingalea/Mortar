#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#include "Char.h"
#include "Mortar.h"

#include "PlayerCtrl.generated.h"

UCLASS(config = Game)
class AMortarPlayerCtrl : public APlayerController
{
	GENERATED_BODY()

public:
	AMortarPlayerCtrl();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	AMortarCharacter* GetChar();
	class AMortarLauncher* GetLauncher();
};

inline AMortarCharacter* AMortarPlayerCtrl::GetChar()
{
	ensure(GetPawn() && !GetPawn()->IsPendingKill());
	return Cast<AMortarCharacter>(GetPawn());
}