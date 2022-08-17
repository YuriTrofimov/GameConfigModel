// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParameterList.h"
#include "UObject/Object.h"
#include "DisplayParameterList.generated.h"

/**
 * Display parameter list
 */
UCLASS(BlueprintType)
class GAMECONFIGMODEL_API UDisplayParameterList : public UParameterList
{
	GENERATED_BODY()
public:
	UDisplayParameterList();
	void CreateVSyncParam();
	void CreateResolutionParam();
	void CreateWindowModeParam();

protected:
	virtual void Initialize(ULocalPlayer* InLocalPlayer) override;
};
