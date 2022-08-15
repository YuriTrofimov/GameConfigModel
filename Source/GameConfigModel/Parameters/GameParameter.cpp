// Fill out your copyright notice in the Description page of Project Settings.


#include "GameParameter.h"

#include "ParameterEditCondition.h"

UGameParameter::UGameParameter()
{
}

void UGameParameter::SetParentParameter(UGameParameter* InParent)
{
	ParentParameter = InParent;
}

void UGameParameter::Initialize(ULocalPlayer* InLocalPlayer)
{
	LocalPlayer = InLocalPlayer;
}

void UGameParameter::AddEditCondition(const TSharedRef<FParameterEditCondition>& InEditCondition)
{
	EditConditions.Add(InEditCondition);
	InEditCondition->OnEditConditionChangedEvent.AddUObject(this, &UGameParameter::OnEditConditionChangedHandler);
}

void UGameParameter::OnEditConditionChangedHandler()
{
	OnParameterEditConditionChangedEvent.Broadcast(this);
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
