// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuroCharacterBase.generated.h"

UCLASS(Abstract)
class AURA_API AAuroCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAuroCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

};
