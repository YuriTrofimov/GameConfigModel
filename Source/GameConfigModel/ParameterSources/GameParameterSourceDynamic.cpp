// Fill out your copyright notice in the Description page of Project Settings.

#include "GameParameterSourceDynamic.h"
#include "GameConfigModel/Parameters/GameParameter.h"

FGameParameterSourceDynamic::FGameParameterSourceDynamic(const TArray<FString>& InDynamicPath) : DynamicPath(InDynamicPath) {}

bool FGameParameterSourceDynamic::Resolve(UGameParameter* InContext)
{
	return DynamicPath.Resolve(InContext);
}

FString FGameParameterSourceDynamic::GetValueAsString(ULocalPlayer* InLocalPlayer) const
{
	FString OutStringValue;
	const bool bSuccess = PropertyPathHelpers::GetPropertyValueAsString(InLocalPlayer, DynamicPath, OutStringValue);
	ensure(bSuccess);
	return OutStringValue;
}

void FGameParameterSourceDynamic::SetValue(ULocalPlayer* InLocalPlayer, const FString& InValue)
{
	const bool bSuccess = PropertyPathHelpers::SetPropertyValueFromString(InLocalPlayer, DynamicPath, InValue);
	ensure(bSuccess);
}

FString FGameParameterSourceDynamic::ToString() const
{
	return DynamicPath.ToString();
}
