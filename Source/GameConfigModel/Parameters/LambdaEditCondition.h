// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "ParameterEditCondition.h"

class UGameParameter;

class FLambdaEditCondition : public FParameterEditCondition
{
public:
	FLambdaEditCondition(TFunction<bool(UGameParameter* InOwningParameter)>&& InCanEditCondition) : CanEditCondition(InCanEditCondition) {}

	virtual bool CanEdit(const ULocalPlayer* InLocalPlayer, UGameParameter* OwnerParameter) override { return CanEditCondition(OwnerParameter); }

private:
	TFunction<bool(UGameParameter* InOwningParameter)> CanEditCondition;
};
