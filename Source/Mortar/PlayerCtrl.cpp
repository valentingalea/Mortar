#include "PlayerCtrl.h"

#include "EngineUtils.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"

#include "Launcher.h"

AMortarPlayerCtrl::AMortarPlayerCtrl()
{
//	LoadConfig(AMortarPlayerCtrl::StaticClass());
}

void AMortarPlayerCtrl::BeginPlay()
{
	Super::BeginPlay();
}

void AMortarPlayerCtrl::SetupInputComponent()
{
	Super::SetupInputComponent();

	{
		FInputAxisBinding AB("MoveForward");
		AB.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float value) {
			GetChar()->MoveForward(value);
		});
		InputComponent->AxisBindings.Emplace(MoveTemp(AB));
	}
	{
		FInputAxisBinding AB("MoveRight");
		AB.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float value) {
			GetChar()->MoveRight(value);
		});
		InputComponent->AxisBindings.Emplace(MoveTemp(AB));
	}

	// "turn" handles devices that provide an absolute delta, such as a mouse.
	{
		FInputAxisBinding AB("Turn");
		AB.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float value) {
			GetChar()->AddControllerYawInput(value);
		});
		InputComponent->AxisBindings.Emplace(MoveTemp(AB));
	}
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	{
		FInputAxisBinding AB("TurnRate");
		AB.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float value) {
			GetChar()->TurnAtRate(value);
		});
		InputComponent->AxisBindings.Emplace(MoveTemp(AB));
	}

	{
		FInputAxisBinding AB("LookUp");
		AB.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float value) {
			GetChar()->AddControllerPitchInput(value);
		});
		InputComponent->AxisBindings.Emplace(MoveTemp(AB));
	}
	{
		FInputAxisBinding AB("LookUpRate");
		AB.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float value) {
			GetChar()->LookUpAtRate(value);
		});
		InputComponent->AxisBindings.Emplace(MoveTemp(AB));
	}

	{
		FInputActionBinding AB("Jump", IE_Pressed);
		AB.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
			GetChar()->Jump();
		});
		InputComponent->AddActionBinding(MoveTemp(AB));
	}
	{
		FInputActionBinding AB("Jump", IE_Released);
		AB.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
			GetChar()->StopJumping();
		});
		InputComponent->AddActionBinding(MoveTemp(AB));
	}

	{
		FInputActionBinding AB("Run", IE_Pressed);
		AB.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
			GetChar()->StartRunning();
		});
		InputComponent->AddActionBinding(MoveTemp(AB));
	}
	{
		FInputActionBinding AB("Run", IE_Released);
		AB.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
			GetChar()->StopRunning();
		});
		InputComponent->AddActionBinding(MoveTemp(AB));
	}

	{
		FInputActionBinding AB("Fire", IE_Pressed);
		AB.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() {
			GetLauncher()->Fire();
		});
		InputComponent->AddActionBinding(MoveTemp(AB));
	}

	{
		FInputAxisBinding AB("Azimuth");
		AB.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float value) {
			GetLauncher()->ModifyAzimuth(value);
		});
		InputComponent->AxisBindings.Emplace(MoveTemp(AB));
	}
	{
		FInputAxisBinding AB("Elevation");
		AB.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float value) {
			GetLauncher()->ModifyElevation(value);
		});
		InputComponent->AxisBindings.Emplace(MoveTemp(AB));
	}
	{
		FInputAxisBinding AB("Velocity");
		AB.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float value) {
			GetLauncher()->ModifyVelocity(value);
		});
		InputComponent->AxisBindings.Emplace(MoveTemp(AB));
	}
}

AMortarLauncher* AMortarPlayerCtrl::GetLauncher()
{
	auto iter = TActorIterator<AMortarLauncher>(GetWorld());
	make_sure(*iter) nullptr; else return *iter;
}
