// Fill out your copyright notice in the Description page of Project Settings.

#include "GameParameter.h"
#include "ParameterEditCondition.h"
#include "GameFramework/GameUserSettings.h"

UGameParameter::UGameParameter()
{
}

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
