// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelSlot.h"
#include "GameConfigModel/ParameterLists/ParameterList.h"
#include "ParameterListWidgetBase.generated.h"

/**
 * Base class for parameter list widget
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMECONFIGMODEL_API UParameterListWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	TSubclassOf<UParameterList> ListClass;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UParameterList> ViewModel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UPanelWidget* ParametersContainer;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
	TSubclassOf<class UGameParameterWidgetBase> GetParameterWidgetClass(UGameParameter* GameParameter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnParameterWidgetCreated(UGameParameterWidgetBase* Widget, UPanelSlot* ContainerSlot);

	UFUNCTION(BlueprintNativeEvent)
	void OnParameterCreated(UGameParameter* Parameter);

public:
	UFUNCTION(BlueprintCallable)
	void SaveCurrentValue();
};
