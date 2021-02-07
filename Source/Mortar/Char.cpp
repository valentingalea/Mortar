#include "Char.h"

#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"

AMortarCharacter::AMortarCharacter()
{
//	LoadConfig(AMortarCharacter::StaticClass());

	// NOTE: expects to be sub-classed in Blueprint with the following hierarchy
	//
	// + Capsule component (root)
	//     |
	//     +-- Arrow component
	//     |
	//     +-- SK Mesh component
	//     |
	//     +-- Camera
	//	   |
	//     .. (others) ...
}

void AMortarCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMortarCharacter::MoveForward(float value)
{
	if (value == 0.f) return;
	AddMovementInput(GetActorForwardVector(), value);
}

void AMortarCharacter::MoveRight(float value)
{
	if (value == 0.f) return;
	AddMovementInput(GetActorRightVector(), value);
}

void AMortarCharacter::TurnAtRate(float rate)
{
	if (rate == 0.f) return;
	AddControllerYawInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMortarCharacter::LookUpAtRate(float rate)
{
	if (rate == 0.f) return;
	AddControllerPitchInput(rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMortarCharacter::StartRunning()
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed *= RunModifier;
}

void AMortarCharacter::StopRunning()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed /= RunModifier;
}