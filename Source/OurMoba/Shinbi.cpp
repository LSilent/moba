// Fill out your copyright notice in the Description page of Project Settings.


#include "Shinbi.h"
#include"Skill.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include"Kismet\GameplayStatics.h"
#include"Particles\ParticleSystem.h"
#include"Components\SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include"Runtime/NavigationSystem/Public/NavigationSystem.h"
#include"Runtime/Engine/Classes/AI/NavigationSystemBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include"Engine\World.h"
#include"CharacterProperty.h"
#include"GhostForm.h"
#include"Buff.h"
#include"DeathFlag.h"
void AShinbi::MulticastSkillEffects_Implementation(UParticleSystem* Particle, FVector EffectLocation)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, Particle, EffectLocation);
}

void AShinbi::ServerSkill1Blink_Implementation(FVector Target)
{
	FVector MyLocation = GetActorLocation();
	FVector Direction = Target - MyLocation;
	FNavLocation  RealPoint;
	Direction.Z = 0.0f;
	if (Direction.Size() > SkillComp->GetSkillRange(0)) return;
	if (!SkillComp->CheckCanBeReleased(0)) return;
	UNavigationSystemV1* NavigationSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavigationSys)
	{
		if (NavigationSys->ProjectPointToNavigation(Target, RealPoint, FVector(500.0f, 500.0f, 500.0f)))
		{
			SkillComp->ReleaseSkill(0);
			MulticastSkillEffects(Skill1React, MyLocation);
			float Radius, Hight;
			GetCapsuleComponent()->GetScaledCapsuleSize(Radius, Hight);
			RealPoint.Location.Z += Hight;
			SetActorLocation(RealPoint.Location);
			float Damage =0.4*PropertyComp->GetCurMagAttack() + SkillComp->GetSkillMagDamage(0);
			auto AllEnemysInRadius = GetAllEnemysInRadiusToLocation(Skill1EffectRange, RealPoint.Location);
			for (auto &i : AllEnemysInRadius)
			{
				i->ReceiveMagDamage(Damage, this);
			}
			AllEnemysInRadius.Empty();
			AllEnemysInRadius = GetAllEnemysInRadius(Skill1EffectRange);
			for (auto &i : AllEnemysInRadius)
			{
				i->ReceiveMagDamage(Damage, this);
			}
			MulticastSkillEffects(Skill1React, RealPoint.Location);
		}
	}
}

bool AShinbi::ServerSkill1Blink_Validate(FVector Target)
{
	return true;
}

void AShinbi::Skill2BecomeGhost()
{
	if (!SkillComp->CheckCanBeReleased(1)) return;
	SkillComp->ReleaseSkill(1);
	AGhostForm* GhostForm= GetWorld()->SpawnActor<AGhostForm>(AGhostForm::StaticClass());
	GhostForm->DeltaAttackSpeed = (0.1 + SkillComp->GetSkillLevel(1)*0.05)*PropertyComp->GetCurAttackSpeed();
	GhostForm->DeltaMoveSpeed = (0.2 + SkillComp->GetSkillLevel(1)*0.1)*PropertyComp->GetCurMoveSpeed();
	BuffComp->AddBuff(GhostForm);
}

void AShinbi::ServerSkill2BecomeGhost_Implementation()
{
	if (!SkillComp->CheckCanBeReleased(1)) return;
	SkillComp->ReleaseSkill(1);
	AGhostForm* GhostForm = GetWorld()->SpawnActor<AGhostForm>(AGhostForm::StaticClass());
	GhostForm->DeltaAttackSpeed = (0.2 + SkillComp->GetSkillLevel(1)*0.1)*PropertyComp->GetCurAttackSpeed();
	GhostForm->DeltaMoveSpeed = (0.2 + SkillComp->GetSkillLevel(1)*0.1)*PropertyComp->GetCurMoveSpeed();
	BuffComp->AddBuff(GhostForm);
}

bool AShinbi::ServerSkill2BecomeGhost_Validate()
{
	return true;
}

void AShinbi::ServerSkill3DeathTarget_Implementation(FVector Target)
{
	FVector MyLocation = GetActorLocation();
	FVector Direction = Target - MyLocation;
	if (Direction.Size() > SkillComp->GetSkillRange(2)) return;
	if (!SkillComp->CheckCanBeReleased(2)) return;
	SkillComp->ReleaseSkill(2);
	MulticastSkillEffects(Skill3React, Target);
	auto AllEnemysInRadius = GetAllEnemysInRadiusToLocation(Skill3EffectRange, Target);
	float Damage = PropertyComp->GetCurMagAttack() + SkillComp->GetSkillMagDamage(2);
	for (auto &i : AllEnemysInRadius)
	{
		if (i->PropertyComp->IsAlive())
		{
			ADeathFlag* DeathFlag = GetWorld()->SpawnActor<ADeathFlag>(ADeathFlag::StaticClass());
			DeathFlag->DeltaMoveSpeed = i->PropertyComp->GetCurMoveSpeed()*(SkillComp->GetSkillLevel(2)*-0.05f - 0.5f);
			DeathFlag->DeltaPhyDef = i->PropertyComp->GetCurPhyDef()*(SkillComp->GetSkillLevel(2)*-0.05f - 0.2f);
			DeathFlag->Attacker = this;
			i->BuffComp->AddBuff(DeathFlag);
			i->ReceiveMagDamage(Damage, this);
		}
	}
}

bool AShinbi::ServerSkill3DeathTarget_Validate(FVector Target)
{
	return true;
}

void AShinbi::Skill3DeathTarget(FVector Target)
{
	FVector MyLocation = GetActorLocation();
	FVector Direction = Target - MyLocation;
	if (Direction.Size() > SkillComp->GetSkillRange(2)) return;
	if (!SkillComp->CheckCanBeReleased(2)) return;
	SkillComp->ReleaseSkill(2);
	MulticastSkillEffects(Skill3React, Target);
	auto AllEnemysInRadius = GetAllEnemysInRadiusToLocation(Skill3EffectRange, Target);
	float Damage = PropertyComp->GetCurMagAttack() + SkillComp->GetSkillMagDamage(2);
	for (auto &i : AllEnemysInRadius)
	{
		if (i->PropertyComp->IsAlive())
		{
			ADeathFlag* DeathFlag = GetWorld()->SpawnActor<ADeathFlag>(ADeathFlag::StaticClass());
			DeathFlag->DeltaMoveSpeed = i->PropertyComp->GetCurMoveSpeed()*(SkillComp->GetSkillLevel(2)*-0.05f - 0.5f);
			DeathFlag->DeltaPhyDef = i->PropertyComp->GetCurPhyDef()*(SkillComp->GetSkillLevel(2)*-0.05f - 0.2f);
			DeathFlag->Attacker = this;
			i->BuffComp->AddBuff(DeathFlag);
			i->ReceiveMagDamage(Damage, this);
		}
	}
}

void AShinbi::Skill1Blink(FVector Target)
{
	FVector MyLocation = GetActorLocation();
	FVector Direction = Target - MyLocation;
	FNavLocation  RealPoint;
	Direction.Z = 0.0f;
	if (Direction.Size() > SkillComp->GetSkillRange(0)) return;
	if (!SkillComp->CheckCanBeReleased(0)) return;
	UNavigationSystemV1* NavigationSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavigationSys)
	{
		if (NavigationSys->ProjectPointToNavigation(Target, RealPoint, FVector(500.0f, 500.0f, 500.0f)))
		{
			SkillComp->ReleaseSkill(0);
			MulticastSkillEffects(Skill1React, MyLocation);
			float Radius, Hight;
			GetCapsuleComponent()->GetScaledCapsuleSize(Radius, Hight);
			RealPoint.Location.Z += Hight;
			SetActorLocation(RealPoint.Location);
			float Damage = 0.4*PropertyComp->GetCurMagAttack() + SkillComp->GetSkillMagDamage(0);
			auto AllEnemysInRadius = GetAllEnemysInRadiusToLocation(Skill1EffectRange, RealPoint.Location);
			for (auto &i : AllEnemysInRadius)
			{
				i->ReceiveMagDamage(Damage, this);
			}
			AllEnemysInRadius.Empty();
			AllEnemysInRadius = GetAllEnemysInRadius(Skill1EffectRange);
			for (auto &i : AllEnemysInRadius)
			{
				i->ReceiveMagDamage(Damage, this);
			}
			MulticastSkillEffects(Skill1React, RealPoint.Location);
		}
	}
}

void AShinbi::Skill1Release()
{
	FVector MouseLocation = GetMouseLocation();
	if (Role == ROLE_Authority)
	{
		Skill1Blink(MouseLocation);
	}
	else
	{
		ServerSkill1Blink(MouseLocation);
	}
}

void AShinbi::Skill2Release()
{
	if (Role == ROLE_Authority)
	{
		Skill2BecomeGhost();
	}
	else if (Role < ROLE_Authority)
	{
		ServerSkill2BecomeGhost();
	}
}

void AShinbi::Skill3Release()
{
	FVector MouseLocation = GetMouseLocation();

	if (Role == ROLE_Authority)
	{
		Skill3DeathTarget(MouseLocation);
	}
	else if (Role < ROLE_Authority)
	{
		ServerSkill3DeathTarget(MouseLocation);
	}
}
