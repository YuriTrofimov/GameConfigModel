// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "GameConfigModel/Parameters/GameParameter.h"
#include "UObject/Interface.h"
#include "ParameterEditorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UParameterEditorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class GAMECONFIGMODEL_API IParameterEditorInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void BindToViewModel(UGameParameter* ViewModel);
};
