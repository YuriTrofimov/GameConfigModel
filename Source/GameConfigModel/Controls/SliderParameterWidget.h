// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "GameParameterWidgetBase.h"
#include "SliderParameterWidget.generated.h"

/**
 * Slider parameter widget for float value with limited range
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class GAMECONFIGMODEL_API USliderParameterWidget : public UGameParameterWidgetBase
{
	GENERATED_BODY()

public:
	virtual void SetGameParameter(UGameParameter* InParameter) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UGameParameter_FloatRange* ViewModel = nullptr;

	UPROPERTY(meta = (BindWidget))
	class USlider* ParameterSlider = nullptr;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSliderValueChanged(float Value);
	void OnViewModelInitialized(UGameParameter* InGameParameter);

	UFUNCTION()
	void OnParameterChangedHandler(UGameParameter* InGameParameter, EGameParameterChangeReason InChangeReason);

};
