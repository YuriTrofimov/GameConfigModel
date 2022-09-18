// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "SliderParameterWidget.h"
#include "Components/Slider.h"
#include "GameConfigModel/Parameters/GameParameter_FloatRange.h"

void USliderParameterWidget::SetGameParameter(UGameParameter* InParameter)
{
	Super::SetGameParameter(InParameter);
	ViewModel = Cast<UGameParameter_FloatRange>(InParameter);
	if (!ViewModel || !ParameterSlider) return;

	ParameterSlider->SetMinValue(ViewModel->GetMinValue());
	ParameterSlider->SetMaxValue(ViewModel->GetMaxValue());
	ParameterSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnSliderValueChanged);
	ViewModel->OnParameterInitialized.AddUObject(this, &ThisClass::OnViewModelInitialized);
}

void USliderParameterWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USliderParameterWidget::OnSliderValueChanged(float Value)
{
	if (!ViewModel) return;
	ViewModel->SetValue(Value);
}

void USliderParameterWidget::OnViewModelInitialized(UGameParameter* InGameParameter)
{
	if (!ViewModel || !ParameterSlider) return;
	ParameterSlider->SetValue(ViewModel->GetValue());
}