﻿// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "GameParameter.h"
#include "ParameterEditCondition.h"
#include "GameFramework/GameUserSettings.h"

UGameParameter::UGameParameter() {}

void UGameParameter::SetDisplayName(const FText& InDisplayName)
{
	DisplayName = InDisplayName;
	OnDisplayNameChanged.Broadcast(this, DisplayName);
}

void UGameParameter::SetDescription(const FText& InDescription)
{
	Description = InDescription;
	OnDescriptionChanged.Broadcast(this, Description);
}

void UGameParameter::SetParentParameter(UGameParameter* InParent)
{
	ParentParameter = InParent;
	ParentParameter->OnParameterChangedEvent.AddUObject(this, &ThisClass::OnParentParameterChangedHandler);
	InParent->AddChildParameter(this);
}

void UGameParameter::IsVisible(bool& Visible)
{
	Visible = bVisible;
}

void UGameParameter::IsEnabled(bool& Enabled)
{
	Enabled = bEnabled;
}

void UGameParameter::Initialize(ULocalPlayer* InLocalPlayer)
{
	if (LocalPlayer == InLocalPlayer) return;

	LocalPlayer = InLocalPlayer;

	// Initialize edit conditions
	for (const TSharedRef<FParameterEditCondition>& EditCondition : EditConditions)
	{
		EditCondition->Initialize(LocalPlayer);
	}

	// Begin async initialization
	BeginInitialize();
}

void UGameParameter::AddEditCondition(const TSharedRef<FParameterEditCondition>& InEditCondition)
{
	EditConditions.Add(InEditCondition);
	InEditCondition->OnEditConditionChangedEvent.AddUObject(this, &UGameParameter::OnEditConditionChangedHandler);
}

void UGameParameter::BeginInitialize()
{
	EndInitialize();
}

void UGameParameter::EndInitialize()
{
	ensureMsgf(!bReady, TEXT("Warning, parameter initialization already completed!"));
	if (!bReady)
	{
		bReady = true;
		OnInitialized();
	}
}

void UGameParameter::OnInitialized()
{
	ensureMsgf(bReady, TEXT("Error! Parameter initialization is not completed!"));
	OnParameterInitialized.Broadcast(this);
}

void UGameParameter::OnEditConditionChangedHandler()
{
	OnParameterEditConditionChangedEvent.Broadcast(this);
}

void UGameParameter::RaiseParameterChanged(EGameParameterChangeReason ChangeReason)
{
	OnParameterChangedEvent.Broadcast(this, ChangeReason);
	for (const TSharedRef<FParameterEditCondition>& Condition : EditConditions)
	{
		Condition->ParameterChanged(LocalPlayer, this, ChangeReason);
	}
}

void UGameParameter::RaiseEditConditionChanged()
{
	OnParameterEditConditionChangedEvent.Broadcast(this);
}

UGameUserSettings* UGameParameter::GetGameUserSettings() const
{
	return CastChecked<UGameUserSettings>(GEngine->GetGameUserSettings());
}

void UGameParameter::AddChildParameter(UGameParameter* InChildParameter)
{
	if (ChildrenParameters.Contains(InChildParameter)) return;

	ChildrenParameters.Add(InChildParameter);
	InChildParameter->OnParameterChangedEvent.AddUObject(this, &ThisClass::OnChildParameterChangedHandler);
}

bool UGameParameter::CanEdit()
{
	if (!LocalPlayer) return false;

	for (const auto& Condition : EditConditions)
	{
		if (!Condition->CanEdit(LocalPlayer, this))
		{
			return false;
		}
	}
	return true;
}

void UGameParameter::OnParentParameterChangedHandler(UGameParameter* InParameter, EGameParameterChangeReason InChangeReason)
{
	const bool bOldEnabled = bEnabled;
	bEnabled = CanEdit();
	if (bOldEnabled != bEnabled)
	{
		OnEnabledChanged.Broadcast(this, bEnabled);
	}
}

void UGameParameter::OnChildParameterChangedHandler(UGameParameter* InParameter, EGameParameterChangeReason InChangeReason) {}
