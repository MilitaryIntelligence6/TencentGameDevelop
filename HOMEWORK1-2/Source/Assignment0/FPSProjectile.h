// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime\Engine\Classes\Components\SphereComponent.h>
#include <Runtime\Engine\Classes\GameFramework\ProjectileMovementComponent.h>
#include "FPSProjectile.generated.h"

UCLASS()
class ASSIGNMENT0_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ������ײ�����
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	// �������˶������
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// �ڷ��䷽�������÷�������ٶȵĺ�����
	void FireInDirection(const FVector& ShootDirection);

	//��������������ʱ���õĺ�����
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit);
};