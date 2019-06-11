// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include"BaseEquipment.h"
#include"Hero.h"
#include"CharacterProperty.h"
#include"Particles\ParticleSystem.h"
#include"Particles\ParticleSystemComponent.h"
// Sets default values for this component's properties
UEquipment::UEquipment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEquipment::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEquipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipment::AddEquipment(ABaseEquipment*NewEquipment)
{
	AHero* OwnerPawn = Cast<AHero>(GetOwner());
	if (OwnerPawn)
	{
		if (AllEquipment.Num() < MaxEquipment)//���ᳬ�����װ������
		{
			if (OwnerPawn->PropertyComp->GetCurMoney() >= NewEquipment->NeedGold)
			{
				NewEquipment->EquipmentIsEffective(OwnerPawn); //��ӽ�ȥ
				AllEquipment.Add(NewEquipment);
				OwnerPawn->PropertyComp->AddMoney(-1 * NewEquipment->NeedGold);
			}
		}
	}
}

void UEquipment::RemoveEquipment(ABaseEquipment*NewEquipment, TArray<ABaseEquipment*>&Arr)
{
	if (Arr.Contains(NewEquipment))
	{
		AHero* OwnerPawn = Cast<AHero>(GetOwner());
		if (OwnerPawn)
		{
			NewEquipment->EndEquipment(OwnerPawn);
			AllEquipment.Remove(NewEquipment);
			OwnerPawn->PropertyComp->AddMoney(NewEquipment->NeedGold*0.6);
		}
	}
}

