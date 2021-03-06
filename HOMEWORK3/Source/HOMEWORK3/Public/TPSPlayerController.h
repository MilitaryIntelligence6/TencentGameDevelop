// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORK3_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

	protected:
	void DeathHandler();

	FTimerHandle DeathTimer;
	public:
	ATPSPlayerController();

	UFUNCTION()
	void OnPlayerDiedHandle();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="PlayerController")
	float RespawnWaitTime;
};
