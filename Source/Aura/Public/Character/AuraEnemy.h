// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuroCharacterBase.h"
#include "Interaction/TargetInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuroCharacterBase, public ITargetInterface
{
	GENERATED_BODY()
	
public:

	AAuraEnemy();


	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
};
