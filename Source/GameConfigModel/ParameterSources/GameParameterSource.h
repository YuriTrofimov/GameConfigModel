// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"

class UGameParameter;

class GAMECONFIGMODEL_API FGameParameterSource : public TSharedFromThis<FGameParameterSource>
{
public:
	virtual ~FGameParameterSource() {}

	/* Await for finish of async paramter intialization */
	virtual void Startup(ULocalPlayer* InLocalPlayer, FSimpleDelegate StartupCompleteCallback);

	virtual bool Resolve(UGameParameter* InContext) = 0;

	virtual FString GetValueAsString(UGameParameter* InContext) const = 0;

	virtual void SetValue(UGameParameter* InContext, const FString& Value) = 0;

	virtual FString ToString() const = 0;
};
