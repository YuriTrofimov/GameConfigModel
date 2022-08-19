// Fill out your copyright notice in the Description page of Project Settings.

#include "ParameterLookup.h"

#define LOCTEXT_NAMESPACE "ParameterLookup"

UParameterLookup::UParameterLookup()
{
}

void UParameterLookup::SelectOptionByIndex(int32 OptionIndex)
{
	if (Options.IsValidIndex(OptionIndex))
	{
		const int32 OptionsCount = Options.Num();
		SelectedIndex = OptionIndex;
		OnSelectionChanged.Broadcast(Options[OptionIndex], OptionIndex, OptionIndex > 0, OptionIndex < OptionsCount - 1);
	}
}

void UParameterLookup::SelectNextOption()
{
	if (SelectedIndex + 1 < Options.Num())
	{
		SelectOptionByIndex(SelectedIndex + 1);
	}
}

void UParameterLookup::SelectPreviousOption()
{
	if (SelectedIndex - 1 > 0)
	{
		SelectOptionByIndex(SelectedIndex - 1);
	}
}

int32 UParameterLookup::GetSelectedIndex()
{
	return SelectedIndex;
}

void UParameterLookup::AddOption(const FString& Value, const FText& Caption)
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!HasOptionValue(Value), TEXT("Option already added InOptionValue: %s InOptionText: %s."), *Value, *Caption.ToString());
#endif

	Options.Add(FParameterLookupOption(Value, Caption));
	RaiseOptionsListChanged();
}

void UParameterLookup::RemoveOption(const FString& InValue)
{
	Options.RemoveAll([InValue](const FParameterLookupOption& CheckOption) { return CheckOption.Value == InValue; });
	RaiseOptionsListChanged();
}

void UParameterLookup::GetOptionsValues(TArray<FString>& Values)
{
	for (const auto& Option : Options)
	{
		Values.Add(Option.Value);
	}
}

void UParameterLookup::GetOptionsCaptions(TArray<FText>& Captions)
{
	for (const auto& Option : Options)
	{
		Captions.Add(Option.Caption);
	}
}

bool UParameterLookup::HasOptionValue(const FString& InOptionValue)
{
	for (const auto& Option : Options)
	{
		if (Option.Value == InOptionValue) return true;
	}
	return false;
}

void UParameterLookup::SetDynamicGetter(const TSharedRef<FGameParameterSource>& InParameterGetter)
{
	ParameterGetter = InParameterGetter;
}

void UParameterLookup::SetDynamicSetter(const TSharedRef<FGameParameterSource>& InParameterSetter)
{
	ParameterSetter = InParameterSetter;
}

FString UParameterLookup::GetValueAsString() const
{
	check(ParameterGetter);
	return ParameterGetter->GetValueAsString(LocalPlayer);
}

void UParameterLookup::SetValueFromString(FString InStringValue)
{
	SetValueFromString(InStringValue, EGameParameterChangeReason::Change);
}

void UParameterLookup::SetDefaultValueFromString(FString InStringValue)
{
	DefaultValue = InStringValue;
}

void UParameterLookup::SaveBaseValue()
{
	BaseValue = GetValueAsString();
}

void UParameterLookup::ResetToDefault()
{
	if (DefaultValue.IsSet())
	{
		SetValueFromString(DefaultValue.GetValue(), EGameParameterChangeReason::ResetToDefault);
	}
}

void UParameterLookup::LoadBaseValue()
{
	SetValueFromString(BaseValue, EGameParameterChangeReason::RestoreToBase);
}

void UParameterLookup::BeginInitialize()
{
	check(ParameterGetter);
	ParameterGetter->Startup(LocalPlayer, FSimpleDelegate::CreateUObject(this, &ThisClass::OnGetterReady));
}

void UParameterLookup::OnGetterReady()
{
	check(ParameterSetter);
	ParameterSetter->Startup(LocalPlayer, FSimpleDelegate::CreateUObject(this, &ThisClass::OnSetterReady));
}

void UParameterLookup::OnSetterReady()
{
	EndInitialize();
}

void UParameterLookup::SetValueFromString(FString InStringValue, EGameParameterChangeReason Reason)
{
	check(ParameterSetter);
	ParameterSetter->SetValue(LocalPlayer, InStringValue);
	RaiseParameterChanged(Reason);
}

void UParameterLookup::OnInitialized()
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

void UParameterLookup::RaiseOptionsListChanged() const
{
	OnOptionsListChanged.Broadcast();
}

UParameterLookup_Enum::UParameterLookup_Enum()
{
	Type = EParameterType::Enum;
}

void UParameterLookup_Enum::OnInitialized()
{
	Super::OnInitialized();
	ensure(Options.Num() > 0);
}

UParameterLookup_Numeric::UParameterLookup_Numeric()
{
	Type = EParameterType::Numeric;
}

void UParameterLookup_Numeric::OnInitialized()
{
	Super::OnInitialized();
	ensure(Options.Num() > 0);
}

UParameterLookup_Bool::UParameterLookup_Bool()
{
	Type = EParameterType::Bool;
	AddOption(TEXT("false"), LOCTEXT("OFF", "OFF"));
	AddOption(TEXT("true"), LOCTEXT("ON", "ON"));
}

void UParameterLookup_Bool::SetValue(bool bNewValue)
{
	SelectOptionByIndex(bNewValue ? 1 : 0);
}

void UParameterLookup_Bool::SetDefaultValue(bool Value)
{
	DefaultValue = LexToString(Value);
}

void UParameterLookup_Bool::SetTrueText(const FText& InText)
{
	RemoveOption(TEXT("true"));
	AddOption(TEXT("true"), InText);
}

void UParameterLookup_Bool::SetFalseText(const FText& InText)
{
	RemoveOption(TEXT("false"));
	AddOption(TEXT("false"), InText);
}

#undef LOCTEXT_NAMESPACE
