#include "Launcher.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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

	PrimaryActorTick.bCanEverTick = true;
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

static float HackInitialVelocity;

void AMortarLauncher::Fire()
{
	const FVector startPos = GetMuzzle()->GetComponentLocation();
	const FRotator startRot = FRotationMatrix::MakeFromX(startPos).Rotator();

	HackInitialVelocity = InitialVelocity;
	TheBall = GetWorld()->SpawnActor<AMortarBall>(startPos, startRot);
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

void AMortarLauncher::ModifyVelocity(float value)
{
	InitialVelocity += value * VelocityChangeRate * GetWorld()->GetDeltaSeconds();
}

void AMortarLauncher::BeginPlay()
{
	Super::BeginPlay();
}

void AMortarLauncher::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

#if 1
// preview trajectory
	const FVector startPos = GetMuzzle()->GetComponentLocation();
	const FVector startVel = startPos.GetUnsafeNormal() * InitialVelocity;
	FPredictProjectilePathParams path(8.f, startPos, startVel, 10.f/*sec*/, ECC_WorldStatic);
	path.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	path.DrawDebugTime = deltaTime;
	FPredictProjectilePathResult result;
	auto valid = UGameplayStatics::PredictProjectilePath(GetWorld(), path, result);
	make_sure(valid);

// draw azimuth/elevation/vel/distance
	const FVector dbgPos = GetBarrel()->GetComponentLocation() + FVector(0, 0, 50);
	const float dist = FVector::Dist(result.PathData.Last().Location, result.PathData[0].Location);
	UKismetSystemLibrary::DrawDebugString(GetWorld(), dbgPos,
		FString::Printf(TEXT("A: %3.0f\nE: %3.0f\nV: %3.0f\n%4.0f m"),
			GetActorRotation().Yaw, GetBarrel()->GetComponentRotation().Roll, InitialVelocity, dist / 100.f),
		nullptr, FLinearColor::White, deltaTime);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////

AMortarBall::AMortarBall()
{
	LoadConfig(AMortarBall::StaticClass());

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("BlockALl");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMortarBall::OnHit);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// this is the best way to create a component from a custom UClass (Blueprint one in this case)
	// calling NewObject() forces to declare an 'outer' and that's a rabbit hole
	MeshComp = static_cast<UStaticMeshComponent*>(CreateDefaultSubobject("MeshComp", MeshClass,
		MeshClass, /*bIsRequired =*/ true, /*bTransient*/ false));
	FAttachmentTransformRules attachmentRules(EAttachmentRule::KeepRelative, false);
	MeshComp->AttachToComponent(RootComponent, attachmentRules);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = HackInitialVelocity;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AMortarBall::OnStop);

	InitialLifeSpan = 0; // forever
}

void AMortarBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AMortarBall::OnStop(const FHitResult& Hit)
{
}