// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "GameParameterSourceDynamic.h"
#include "GameConfigModel/Parameters/GameParameter.h"

#pragma optimize("", off)

FGameParameterSourceDynamic::FGameParameterSourceDynamic(const TArray<FString>& InDynamicPath)
	: DynamicPath(InDynamicPath)
{
}

bool FGameParameterSourceDynamic::Resolve(UGameParameter* InContext)
{
	return DynamicPath.Resolve(InContext);
}

FString FGameParameterSourceDynamic::GetValueAsString(UGameParameter* InContext)
{
	FString OutStringValue;
	const bool bSuccess = PropertyPathHelpers::GetPropertyValueAsString(InContext, DynamicPath, OutStringValue);
	ensure(bSuccess);
	return OutStringValue;
}

void FGameParameterSourceDynamic::SetValue(UGameParameter* InContext, const FString& InValue)
{
	const bool bSuccess = PropertyPathHelpers::SetPropertyValueFromString(InContext, DynamicPath, InValue);
	ensure(bSuccess);
}

FString FGameParameterSourceDynamic::ToString() const
{
	return DynamicPath.ToString();
}

#pragma optimize("", on)
