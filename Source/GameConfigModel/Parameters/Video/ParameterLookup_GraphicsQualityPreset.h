// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "GameConfigModel/Parameters/ParameterLookup.h"
#include "ParameterLookup_GraphicsQualityPreset.generated.h"

/**
 * Graphics Quality Preset parameter lookup
 */
UCLASS()
class GAMECONFIGMODEL_API UParameteLookup_GraphicsQualityPreset : public UParameterLookup_Numeric
{
	GENERATED_BODY()
public:
	UParameteLookup_GraphicsQualityPreset();

	virtual void SelectOptionByIndex(int32 OptionIndex) override;
	virtual void SelectOptionByValue(const FString& InValue, EGameParameterChangeReason Reason) override;
	virtual void ResetToDefault() override;
	virtual void LoadBaseValue() override;
	virtual void SaveBaseValue() override;
	virtual void GetOptionsCaptions(TArray<FText>& Captions) override;
	virtual int32 GetSelectedIndex() override;
protected:
	bool bCustomMode = false;
	int32 CustomIndex = 4;
	FParameterLookupOption CustomOption;
	virtual void BeginInitialize() override;
	virtual void OnInitialized() override;
	virtual void OnChildParameterChangedHandler(UGameParameter* InParameter, EGameParameterChangeReason InChangeReason) override;
	int32 GetOverallQualityLevel() const;
};
