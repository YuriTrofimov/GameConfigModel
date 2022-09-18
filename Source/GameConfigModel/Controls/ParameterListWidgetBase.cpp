// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "ParameterListWidgetBase.h"

#include "GameParameterWidgetBase.h"
#include "Components/PanelWidget.h"

void UParameterListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ListClass) return;
	ViewModel = NewObject<UParameterList>(this, ListClass);
	if (!ViewModel) return;
	ViewModel->OnParameterCreated.AddDynamic(this, &ThisClass::OnParameterCreated);
	ViewModel->InitializeList(GetOwningPlayer());
	ViewModel->LoadBaseValue();
}

void UParameterListWidgetBase::SaveCurrentValue()
{
	if (!ViewModel) return;
	ViewModel->SaveBaseValue();
}

void UParameterListWidgetBase::OnParameterCreated_Implementation(UGameParameter* Parameter)
{
	if (!ParametersContainer) return;

	const TSubclassOf<UGameParameterWidgetBase> ParamWidgetClass = GetParameterWidgetClass(Parameter);
	if (!ParamWidgetClass) return;
	auto* ParamWidget = CreateWidget<UGameParameterWidgetBase>(GetOwningPlayer(), ParamWidgetClass);
	if (!ParamWidget) return;
	ParamWidget->SetGameParameter(Parameter);
	auto* ContainerSlot = ParametersContainer->AddChild(ParamWidget);
	OnParameterWidgetCreated(ParamWidget, ContainerSlot);
}
