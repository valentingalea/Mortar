#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Launcher.generated.h"

UENUM(BlueprintType)
enum class ELauncherMode : uint8
{
	Setup,
	InFlight
};

class UStaticMeshComponent;

UCLASS()
class AMortarLauncher : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup)
	ELauncherMode Mode = ELauncherMode::Setup;

	/** deg/sec */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
	float AzymuthChangeRate = 1.f;

	/** deg/sec */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
	float ElevationChangeRate = 1.f;

	/** cm/sec */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
	float InitialVelocity = 1000.f;

	/** cm/sec */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
	float VelocityChangeRate = 500.f;

public:
	AMortarLauncher();

	void Fire();
	void ModifyAzimuth(float);
	void ModifyElevation(float);
	void ModifyVelocity(float);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float) override;

	class AMortarBall* TheBall;
	UStaticMeshComponent* GetBarrel();
	USceneComponent* GetMuzzle();
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Config = Game)
class AMortarBall : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Setup)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Setup)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(Config, VisibleDefaultsOnly, BlueprintReadOnly, Category = Setup)
	UClass* MeshClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Setup)
	UProjectileMovementComponent* ProjectileMovement;

public:
	AMortarBall();

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnStop(const FHitResult& Hit);
};
