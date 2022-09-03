// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "GameParameter_FloatRange.h"

UGameParameter_FloatRange::UGameParameter_FloatRange()
{
}

void UGameParameter_FloatRange::SetMinValue(float InMinValue)
{
	MinValue = InMinValue;
}

void UGameParameter_FloatRange::SetMaxValue(float InMaxValue)
{
	MaxValue = InMaxValue;
}

void UGameParameter_FloatRange::SetValue(float InValue, EGameParameterChangeReason Reason)
{
	InValue = FMath::Clamp(InValue, MinValue, MaxValue);
	Super::SetValue(InValue, Reason);
}
