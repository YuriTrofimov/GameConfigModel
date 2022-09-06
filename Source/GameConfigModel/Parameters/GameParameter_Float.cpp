// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "GameParameter_Float.h"
#include "GameConfigModel/ParameterSources/GameParameterSource.h"

UGameParameter_Float::UGameParameter_Float()
{
	Type = EParameterType::Numeric;
}

float UGameParameter_Float::GetValue()
{
	if (!bReady) return 0.0f;
	check(ParameterGetter);
	const FString StringVal = ParameterGetter->GetValueAsString(this);
	float Result = 0.0f;
	LexFromString(Result, *StringVal);
	return Result;
}

void UGameParameter_Float::SetValue(float InValue)
{
	SetValue(InValue, EGameParameterChangeReason::Change);
}

void UGameParameter_Float::SetValue(float InValue, EGameParameterChangeReason Reason)
{
	if (!bReady) return;
	const FString StringVal = LexToString(InValue);
	check(ParameterSetter);
	ParameterSetter->SetValue(this, StringVal);
	RaiseParameterChanged(Reason);
}

void UGameParameter_Float::SetDynamicGetter(const TSharedRef<FGameParameterSource>& InParameterGetter)
{
	ParameterGetter = InParameterGetter;
}

void UGameParameter_Float::SetDynamicSetter(const TSharedRef<FGameParameterSource>& InParameterSetter)
{
	ParameterSetter = InParameterSetter;
}

void UGameParameter_Float::ResetToDefault()
{
	if (DefaultValue.IsSet())
	{
		SetValue(DefaultValue.GetValue(), EGameParameterChangeReason::ResetToDefault);
	}
}

void UGameParameter_Float::BeginInitialize()
{
	check(ParameterGetter);
	ParameterGetter->Startup(LocalPlayer, FSimpleDelegate::CreateUObject(this, &ThisClass::OnGetterReady));
}

void UGameParameter_Float::SetDefaultValue(float InDefaultValue)
{
	DefaultValue = InDefaultValue;
}

void UGameParameter_Float::SaveBaseValue()
{
	SetValue(BaseValue);
}

void UGameParameter_Float::LoadBaseValue()
{
	BaseValue = GetValue();
}

void UGameParameter_Float::OnGetterReady()
{
	check(ParameterSetter);
	ParameterSetter->Startup(LocalPlayer, FSimpleDelegate::CreateUObject(this, &ThisClass::OnSetterReady));
}

void UGameParameter_Float::OnSetterReady()
{
	EndInitialize();
}

void UGameParameter_Float::OnInitialized()
{
#if !UE_BUILD_SHIPPING
	ensureAlways(ParameterGetter);
	ensureAlwaysMsgf(ParameterGetter->Resolve(this),
		TEXT("%s: %s did not resolve, are all functions and properties valid, and are they UFunctions/UProperties? Does the getter function have no parameters?"), *UniqueName.ToString(),
		*ParameterGetter->ToString());
	ensureAlways(ParameterSetter);
	ensureAlwaysMsgf(ParameterSetter->Resolve(this),
		TEXT("%s: %s did not resolve, are all functions and properties valid, and are they UFunctions/UProperties? Does the setting function have exactly one parameter?"), *UniqueName.ToString(),
		*ParameterSetter->ToString());
#endif

	Super::OnInitialized();
}
