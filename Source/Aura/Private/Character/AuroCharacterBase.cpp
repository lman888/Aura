// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuroCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Aura/Aura.h"

AAuroCharacterBase::AAuroCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

UAbilitySystemComponent* AAuroCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuroCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuroCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AAuroCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();
	bDead = true;
}

// Called when the game starts or when spawned
void AAuroCharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}

void AAuroCharacterBase::InitAbilityActorInfo()
{
}

FVector AAuroCharacterBase::GetCombatSocketLocation_Implementation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

bool AAuroCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuroCharacterBase::GetAvatar_Implementation()
{
	return this;
}

void AAuroCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle Context = GetAbilitySystemComponent()->MakeEffectContext();
	Context.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, Context);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuroCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);
}

void AAuroCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority())
	{
		return;
	}
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	AuraASC->AddCharacterAbilities(StartupAbilities);
}

void AAuroCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstace))
	{
		UMaterialInstanceDynamic* DynamicMatIsnt = UMaterialInstanceDynamic::Create(DissolveMaterialInstace, this);
		GetMesh()->SetMaterial(0, DynamicMatIsnt);
		StartDissolveTimeline(DynamicMatIsnt);
	}

	if (IsValid(WeaponDissolveMaterialInstace))
	{
		UMaterialInstanceDynamic* DynamicMatIsnt = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstace, this);
		Weapon->SetMaterial(0, DynamicMatIsnt);
		StartWeaponDissolveTimeline(DynamicMatIsnt);
	}
}
