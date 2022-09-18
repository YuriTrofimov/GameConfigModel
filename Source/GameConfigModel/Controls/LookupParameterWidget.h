// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "GameParameterWidgetBase.h"
#include "GameConfigModel/Parameters/ParameterLookup.h"
#include "LookupParameterWidget.generated.h"

/**
 * Base widget for lookup type parameter
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class GAMECONFIGMODEL_API ULookupParameterWidget : public UGameParameterWidgetBase
{
	GENERATED_BODY()

public:
	virtual void SetGameParameter(class UGameParameter* InParameter);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UParameterLookup> ViewModel;

	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* ButtonNext = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ButtonPrevious = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	class UComboBoxString* OptionsCombo = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ValueCaption = nullptr;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnNextButtonClicked();

	UFUNCTION()
	void OnPreviousButtonClicked();

	UFUNCTION(BlueprintNativeEvent)
	void OnSelectionChanged(FParameterLookupOption Option, int SelectedIndex, bool bCanBack, bool bCanForward);

	UFUNCTION(BlueprintNativeEvent)
	void OnOptionsListChanged();

private:
	UFUNCTION()
	void OnSelectionChangedHandler(FParameterLookupOption Option, int SelectedIndex, bool bCanBack, bool bCanForward);

	UFUNCTION()
	void OnOptionsListChangedHandler();
};
