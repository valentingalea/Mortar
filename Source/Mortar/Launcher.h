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

UCLASS(Config = Game)
class AMortarLauncher : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setup)
	ELauncherMode Mode = ELauncherMode::Setup;

	UPROPERTY(Config, VisibleDefaultsOnly, BlueprintReadOnly, Category = Setup)
	UClass* BallClass;

	/** deg/sec */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
	float AzymuthChangeRate = 1.f;

	/** deg/sec */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setup)
	float ElevationChangeRate = 1.f;

public:
	AMortarLauncher();

	void Fire();
	void ModifyAzimuth(float);
	void ModifyElevation(float);

protected:
	virtual void BeginPlay() override;

	class AMortarBall* TheBall;
	UStaticMeshComponent* GetBarrel();
	USceneComponent* GetMuzzle();
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AMortarBall : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	UProjectileMovementComponent* ProjectileMovement;

public:
	AMortarBall();

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnStop(const FHitResult& Hit);
};
