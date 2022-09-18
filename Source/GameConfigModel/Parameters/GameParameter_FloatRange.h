// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "GameParameter_Float.h"
#include "GameParameter_FloatRange.generated.h"

/**
 * Float type game parameter. Value is limited in MinValue-MaxValue range
 */
UCLASS()
class GAMECONFIGMODEL_API UGameParameter_FloatRange : public UGameParameter_Float
{
	GENERATED_BODY()
public:
	UGameParameter_FloatRange();

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMinValue() const { return MinValue; }

	UFUNCTION(BlueprintCallable)
	void SetMinValue(float InMinValue);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxValue() const { return MaxValue; }

	UFUNCTION(BlueprintCallable)
	void SetMaxValue(float InMaxValue);

	virtual void SetValue(float InValue) override;

protected:
	float MinValue = 0.0f;
	float MaxValue = 1.0f;
	virtual void SetValue(float InValue, EGameParameterChangeReason Reason) override;
};
