// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

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
