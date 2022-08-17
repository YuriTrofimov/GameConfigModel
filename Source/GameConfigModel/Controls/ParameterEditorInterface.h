// Fill out your copyright notice in the Description page of Project Settings.

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
