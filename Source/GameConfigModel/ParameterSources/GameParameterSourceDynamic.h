// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameParameterSource.h"
#include "PropertyPathHelpers.h"

/**
 *
 */
class GAMECONFIGMODEL_API FGameParameterSourceDynamic : public FGameParameterSource
{
public:
	FGameParameterSourceDynamic(const TArray<FString>& InDynamicPath);

	virtual bool Resolve(UGameParameter* InContext) override;

	virtual FString GetValueAsString(ULocalPlayer* InLocalPlayer) const override;

	virtual void SetValue(ULocalPlayer* InLocalPlayer, const FString& InValue) override;

	virtual FString ToString() const override;

private:
	FCachedPropertyPath DynamicPath;
};
