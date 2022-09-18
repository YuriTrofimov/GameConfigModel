// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "ParameterList.h"
#include "VideoQualityParameterList.generated.h"

/**
 * Video Qualtity parameters list
 */
UCLASS()
class GAMECONFIGMODEL_API UVideoQualityParameterList : public UParameterList
{
	GENERATED_BODY()
public:
	UVideoQualityParameterList();

	virtual void ResetToDefault() override;

protected:
	virtual void Initialize(ULocalPlayer* InLocalPlayer) override;
	UPROPERTY()
	TObjectPtr<UGameParameter> PresetParameter;
	virtual UGameParameter* CreateGraphicsPreset();
	virtual UGameParameter* CreateGlobalIlluminationQuality(UGameParameter* InQualityPresetParameter);
	virtual UGameParameter* CreateShadowsQuality(UGameParameter* InQualityPresetParameter);
	virtual UGameParameter* CreateAntiAliasingQuality(UGameParameter* InQualityPresetParameter);
	virtual UGameParameter* CreateViewDistance(UGameParameter* InQualityPresetParameter);
	virtual UGameParameter* CreateTextureQuality(UGameParameter* InQualityPresetParameter);
	virtual UGameParameter* CreateVisualEffectQuality(UGameParameter* InQualityPresetParameter);
	virtual UGameParameter* CreateReflectionQuality(UGameParameter* InQualityPresetParameter);
	virtual UGameParameter* CreatePostProcessingQuality(UGameParameter* InQualityPresetParameter);
};
