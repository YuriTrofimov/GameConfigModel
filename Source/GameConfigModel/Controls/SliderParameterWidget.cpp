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
	ViewModel->OnParameterChangedEvent.AddUObject(this, &ThisClass::OnParameterChangedHandler);
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

void USliderParameterWidget::OnParameterChangedHandler(UGameParameter* InGameParameter, EGameParameterChangeReason InChangeReason)
{
	if (!ViewModel || !ParameterSlider) return;
	if (InChangeReason != EGameParameterChangeReason::Change)
	{
		const float Value = ViewModel->GetValue();
		ParameterSlider->SetValue(Value);
	}
}
