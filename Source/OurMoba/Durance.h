// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBuff.h"
#include "Durance.generated.h"

/**
 * 
 */
UCLASS()
class OURMOBA_API ADurance : public ABaseBuff
{
	GENERATED_BODY()
public:
		ADurance();
		virtual bool BuffIsEffective(ABaseCharacter* OwnerPawn)override;
};
