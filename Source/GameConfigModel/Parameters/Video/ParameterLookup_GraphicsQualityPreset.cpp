// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "ParameterLookup_GraphicsQualityPreset.h"
#include "GameFramework/GameUserSettings.h"

#define LOCTEXT_NAMESPACE "GameParameters"

UParameteLookup_GraphicsQualityPreset::UParameteLookup_GraphicsQualityPreset()
{
	Type = EParameterType::Custom;
	CustomOption = FParameterLookupOption(FString::FromInt(CustomIndex),LOCTEXT("GraphicsQualityPreset_Custom", "Custom"));
}

void UParameteLookup_GraphicsQualityPreset::SelectOptionByIndex(int32 OptionIndex)
{
	UGameUserSettings* UserSettings = CastChecked<UGameUserSettings>(GEngine->GetGameUserSettings());
	if (OptionIndex != CustomIndex)
	{
		// Low / Medium / High / Epic
		UserSettings->SetOverallScalabilityLevel(OptionIndex);
		Super::SelectOptionByIndex(OptionIndex);
		RaiseParameterChanged(EGameParameterChangeReason::Change);
	}
	else
	{
		OnSelectionChanged.Broadcast(CustomOption, CustomIndex, true, false);
	}
}

void UParameteLookup_GraphicsQualityPreset::SelectOptionByValue(const FString& InValue, EGameParameterChangeReason Reason)
{
	if (InValue == CustomOption.Value)
	{
		SelectOptionByIndex(CustomIndex);
	}
	else
	{
		Super::SelectOptionByValue(InValue, Reason);
	}
}

void UParameteLookup_GraphicsQualityPreset::ResetToDefault()
{
	const int32 OverallQualityLevel = GetOverallQualityLevel();
	SelectOptionByIndex(OverallQualityLevel);
}

void UParameteLookup_GraphicsQualityPreset::LoadBaseValue()
{
	const int32 OverallQualityLevel = GetOverallQualityLevel();
	if (OverallQualityLevel != INDEX_NONE)
	{
		BaseValue = FString::FromInt(OverallQualityLevel);
	}
	else
	{
		BaseValue = FString::FromInt(CustomIndex);
	}
}

void UParameteLookup_GraphicsQualityPreset::SaveBaseValue()
{
}

void UParameteLookup_GraphicsQualityPreset::GetOptionsCaptions(TArray<FText>& Captions)
{
	for (const auto& Option : Options)
	{
		Captions.Add(Option.Caption);
	}

	const int32 OverallQualityLevel = GetOverallQualityLevel();
	if (OverallQualityLevel == INDEX_NONE)
	{
		Captions.Add(CustomOption.Caption);
	}
}

int32 UParameteLookup_GraphicsQualityPreset::GetSelectedIndex()
{
	const int32 OverallQualityLevel = GetOverallQualityLevel();
	if (OverallQualityLevel == INDEX_NONE)
	{
		return CustomIndex;
	}
	return OverallQualityLevel;
}

void UParameteLookup_GraphicsQualityPreset::BeginInitialize()
{
	EndInitialize();
}

void UParameteLookup_GraphicsQualityPreset::OnInitialized()
{
	AddNumericOption(0, LOCTEXT("GraphicsQualityPreset_Low", "Low"));
	AddNumericOption(1, LOCTEXT("GraphicsQualityPreset_Medium", "Medium"));
	AddNumericOption(2, LOCTEXT("GraphicsQualityPreset_High", "High"));
	AddNumericOption(3, LOCTEXT("GraphicsQualityPreset_Epic", "Epic"));
	LoadBaseValue();
	SelectOptionByValue(BaseValue, EGameParameterChangeReason::RestoreToBase);
}

void UParameteLookup_GraphicsQualityPreset::OnChildParameterChangedHandler(UGameParameter* InParameter, EGameParameterChangeReason InChangeReason)
{
	if (InChangeReason == EGameParameterChangeReason::Change)
	{
		LoadBaseValue();
		SelectOptionByValue(BaseValue, EGameParameterChangeReason::DependencyChanged);
	}
}

int32 UParameteLookup_GraphicsQualityPreset::GetOverallQualityLevel() const
{
	const UGameUserSettings* UserSettings = CastChecked<const UGameUserSettings>(GEngine->GetGameUserSettings());
	return UserSettings->GetOverallScalabilityLevel();
}

#undef LOCTEXT_NAMESPACE
