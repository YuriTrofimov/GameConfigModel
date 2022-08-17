// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "ParameterEditCondition.generated.h"

class UGameParameter;

/**
 *
 */
class GAMECONFIGMODEL_API FParameterEditCondition : public TSharedFromThis<FParameterEditCondition>
{
public:
	FParameterEditCondition(){};

	virtual ~FParameterEditCondition(){};

	DECLARE_EVENT(FParameterEditCondition, FOnConditionChanged);

	FOnConditionChanged OnEditConditionChangedEvent;

	/* Initialize edit condition */
	virtual void Initialize(const ULocalPlayer* InLocalPlayer) {}

	/* Owner parameter applied */
	virtual void ParameterApplied(const ULocalPlayer* InLocalPlayer, UGameParameter* OwnerParameter) const {}

	/* Owner parameter changed */
	virtual void ParameterChanged(const ULocalPlayer* InLocalPlayer, UGameParameter* OwnerParameter, enum class EGameParameterChangeReason Reason) const {}

	/* Return TRUE if owner parameter can be edited */
	virtual bool CanEdit(const ULocalPlayer* InLocalPlayer, UGameParameter* OwnerParameter) { return true; }

protected:
	/* Raises OnEditConditionChangedEvent */
	void RaiseConditionChanged() const { OnEditConditionChangedEvent.Broadcast(); }
};
