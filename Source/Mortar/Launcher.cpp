#include "Launcher.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Mortar.h"

AMortarLauncher::AMortarLauncher()
{
	auto root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	// NOTE: expects to be sub-classed in Blueprint with the following hierarchy
	//
	// + RootComponent
	//     |
	//     +-- Base plate mesh
	//     |
	//     +-- Barrel mesh
	//         |
	//         +-- Muzzle component for parenting the ball
	//	       

	LoadConfig(AMortarLauncher::StaticClass());
}

UStaticMeshComponent* AMortarLauncher::GetBarrel()
{
	auto list = GetComponentsByTag(UStaticMeshComponent::StaticClass(), "barrel");
	make_sure(list.Num()) nullptr; else return Cast<UStaticMeshComponent>(list[0]);
}

USceneComponent* AMortarLauncher::GetMuzzle()
{
	auto list = GetComponentsByTag(USceneComponent::StaticClass(), "muzzle");
	make_sure(list.Num()) nullptr; else return Cast<USceneComponent>(list[0]);
}

void AMortarLauncher::Fire()
{
	const FVector startPos = GetMuzzle()->GetComponentLocation();
	const FRotator startRot = FRotationMatrix::MakeFromX(startPos).Rotator();

	TheBall = GetWorld()->SpawnActor<AMortarBall>(BallClass, startPos, startRot);
	make_sure(TheBall);
}

void AMortarLauncher::ModifyAzimuth(float value)
{
	if (value == 0.f) return;
	FRotator R = GetActorRotation();
	R.Yaw += value * AzymuthChangeRate * GetWorld()->GetDeltaSeconds();
	SetActorRotation(R);
}

void AMortarLauncher::ModifyElevation(float value)
{
	if (value == 0.f) return;
	FRotator R = GetBarrel()->GetComponentRotation();
	R.Roll -= value * ElevationChangeRate * GetWorld()->GetDeltaSeconds();
	GetBarrel()->SetWorldRotation(R);
}


void AMortarLauncher::BeginPlay()
{
	make_sure(BallClass);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

AMortarBall::AMortarBall()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("BlockALl");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMortarBall::OnHit);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AMortarBall::OnStop);

	InitialLifeSpan = 0; // forever
}

void AMortarBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AMortarBall::OnStop(const FHitResult& Hit)
{
}