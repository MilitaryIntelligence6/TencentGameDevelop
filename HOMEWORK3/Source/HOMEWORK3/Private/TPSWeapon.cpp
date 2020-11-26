// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials//PhysicalMaterial.h"
#include "../HOMEWORK3.h"

// Sets default values
ATPSWeapon::ATPSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	ShootRange = 10000.0;
	BaseDamage = 20;

	MuzzleSocketName = "Muzzle";
	TraceSocketName = "Target";
}

// Called when the game starts or when spawned
void ATPSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPSWeapon::Fire()
{
	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner) {
		FVector EyeLocation;
		FRotator EyeRotation;
		WeaponOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector TraceBegin = EyeLocation;
		FVector	TraceEnd = EyeLocation + (EyeRotation.Vector() * ShootRange);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(WeaponOwner);
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.bTraceComplex = true; // 碰撞精准到Triangle Mesh
		CollisionQueryParams.bReturnPhysicalMaterial = true; // 此处需要设置为真，否则默认不获取物理材质

		FHitResult HitResult;// 存储碰撞的结果
		EPhysicalSurface HitSurfaceType = SURFACETYPE_DEFAULT;

		float ActualDamge = BaseDamage;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceBegin, TraceEnd, ECC_Visibility, CollisionQueryParams)) {
			HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
			
			if (HitSurfaceType == SURFACETYPE_DEFAULT) {
				ActualDamge *= 1.5;
			}

			ApplyDamage(HitResult.GetActor(),ActualDamge, EyeRotation.Vector(), HitResult, WeaponOwner->GetInstigatorController());

			TraceEnd = HitResult.ImpactPoint;

			PlayImpactEffect(HitSurfaceType, TraceEnd);
		}

		PlayFireEffect(TraceEnd);
	}
}

void ATPSWeapon::ApplyDamage(AActor* HitedActor,float damage, FVector HitFromDirection, FHitResult HitInfo, AController* EventInsitigator)
{
	if (HitedActor) {
		UGameplayStatics::ApplyPointDamage(HitedActor, damage, HitFromDirection, HitInfo, EventInsitigator, this, DamageType);
	}
}

void ATPSWeapon::PlayFireEffect(FVector TraceEnd)
{

	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, MuzzleLocation);

	if (MuzzleEffect) {// 设置开火特效
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	// 设置弹道轨迹
	if (TracerEffect) {
		UParticleSystemComponent* TraceComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TraceComp) {
			TraceComp->SetVectorParameter(TraceSocketName, TraceEnd);// 设置目的点为击中的位置
		}
	}
}

void ATPSWeapon::PlayImpactEffect(EPhysicalSurface SurfaceType, FVector TraceEnd)
{
	// 根据命中的物理材质不同，选择不同的粒子系统
	UParticleSystem* SelectedEffect = nullptr;
	USoundBase* SelectSound = nullptr;
	switch (SurfaceType) {
	case SURFACETYPE_DEFAULT:
		SelectedEffect = DefaultImpactEffect;
		SelectSound = DefaultSound;
		break;
	case SURFACETYPE_FLESHDEFAULT:
		SelectedEffect = FleshDefaultImpactEffect;
		SelectSound = HitBodySound;
		break;
	case SURFACETYPE_FLESHHEAD:
		SelectedEffect = FleshHeadImpactEffect;
		SelectSound = HeadBoostSound;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		SelectSound = DefaultSound;
		break;
	}
	
	if (SelectedEffect) {
		// 计算射击方向
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector ShootDirection = TraceEnd - MuzzleLocation;
		ShootDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, TraceEnd,ShootDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SelectSound, TraceEnd);
	}
}

// Called every frame
void ATPSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

