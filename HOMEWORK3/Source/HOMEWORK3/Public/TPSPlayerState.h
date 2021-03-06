// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORK3_API ATPSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerState")
	FText PlayerName;

	UPROPERTY(Replicated)
	int PlayerKillNum;

	UPROPERTY(Replicated)
	int PlayerDiedNum;

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void AddScore(float ScoreDelta);

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void SetName(FString Name);
	
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	FString GetName()const;

	UFUNCTION(BlueprintCallable)
	int GetPlayerKillNum() const;

	UFUNCTION(BlueprintCallable)
	int GetPlayerDiedNum() const;

	UFUNCTION(BlueprintCallable)
	void AddPlayerKillNum();

	UFUNCTION(BlueprintCallable)
	void AddPlayerDiedNum();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
};
