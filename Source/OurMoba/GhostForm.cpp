// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostForm.h"
#include"BaseCharacter.h"
#include"CharacterProperty.h"
#include"Particles\ParticleSystemComponent.h"
#include"UObject/ConstructorHelpers.h"
AGhostForm::AGhostForm()
{
	SustainTime = 8.0f;
	bIsUnique = true;
	Type = BuffType::GhostForm;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemMaterialAsset(TEXT("ParticleSystem'/Game/Game/Particle/P_Recall_Shadowplane.P_Recall_Shadowplane'"));
	if (ParticleSystemMaterialAsset.Succeeded())
	{
		React->Template = ParticleSystemMaterialAsset.Object;
	}
}
bool AGhostForm::BuffIsEffective(ABaseCharacter * OwnerPawn)
{
	if (OwnerPawn)
	{
		Receiver = OwnerPawn;
		UCharacterProperty* MyProperty = OwnerPawn->PropertyComp;
		MyProperty->AddCurAttackSpeed(DeltaAttackSpeed);
		MyProperty->AddCurMoveSpeed(DeltaMoveSpeed);
		React->SetVisibility(true);
		return true;
	}
	return false;
}