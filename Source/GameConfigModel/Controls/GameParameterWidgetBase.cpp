// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "GameParameterWidgetBase.h"
#include "Components/TextBlock.h"
#include "GameConfigModel/Parameters/GameParameter.h"

void UGameParameterWidgetBase::SetGameParameter(UGameParameter* InParameter)
{
	if (!InParameter) return;
	GameParameter = InParameter;
	GameParameter->OnDisplayNameChanged.AddDynamic(this, &ThisClass::OnDisplayNameChangedHandler);
	GameParameter->OnDescriptionChanged.AddDynamic(this, &ThisClass::OnDescriptionChangedHandler);
	GameParameter->OnEnabledChanged.AddDynamic(this, &ThisClass::OnEnabledChangedHandler);
	if (ParameterCaption)
	{
		ParameterCaption->SetText(GameParameter->GetDisplayName());
		SetToolTipText(GameParameter->GetDescription());
		bool bParamEnabled = false;
		GameParameter->IsEnabled(bParamEnabled);
		SetIsEnabled(bParamEnabled);
	}
}

void UGameParameterWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameParameterWidgetBase::OnDisplayNameChanged_Implementation(UGameParameter* Parameter, const FText& Text)
{
	if (!ParameterCaption) return;
	ParameterCaption->SetText(Text);
}

void UGameParameterWidgetBase::OnDescriptionChanged_Implementation(UGameParameter* Parameter, const FText& Text)
{
	SetToolTipText(Text);
}

void UGameParameterWidgetBase::OnEnabledChanged_Implementation(UGameParameter* Parameter, bool bIsParameterEnabled)
{
	SetIsEnabled(bIsParameterEnabled);
}

void UGameParameterWidgetBase::OnDisplayNameChangedHandler(UGameParameter* Parameter, FText Text)
{
	OnDisplayNameChanged(Parameter, Text);
}

void UGameParameterWidgetBase::OnDescriptionChangedHandler(UGameParameter* Parameter, FText Text)
{
	OnDescriptionChanged(Parameter, Text);
}

void UGameParameterWidgetBase::OnEnabledChangedHandler(UGameParameter* Parameter, bool bIsParameterEnabled)
{
	OnEnabledChanged(Parameter, bIsEnabled);
}
