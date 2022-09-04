// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "ParameterList.h"
#include "UObject/Object.h"
#include "DisplayParameterList.generated.h"

class UParameterLookup_Enum;

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
	void CreateResolutionParam(UParameterLookup_Enum* WindowModeParameter);
	UParameterLookup_Enum* CreateWindowModeParam();

protected:
	virtual void Initialize(ULocalPlayer* InLocalPlayer) override;
};
