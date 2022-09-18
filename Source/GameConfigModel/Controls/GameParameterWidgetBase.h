// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameParameterWidgetBase.generated.h"

/**
 * Base class for game parameter widget
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class GAMECONFIGMODEL_API UGameParameterWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SetGameParameter(class UGameParameter* InParameter);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UGameParameter> GameParameter;

	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* ParameterCaption = nullptr;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnDisplayNameChanged(UGameParameter* Parameter, const FText& Text);

	UFUNCTION(BlueprintNativeEvent)
	void OnDescriptionChanged(UGameParameter* Parameter, const FText& Text);

	UFUNCTION(BlueprintNativeEvent)
	void OnEnabledChanged(UGameParameter* Parameter, bool bIsParameterEnabled);

private:
	UFUNCTION()
	void OnDisplayNameChangedHandler(UGameParameter* Parameter, FText Text);

	UFUNCTION()
	void OnDescriptionChangedHandler(UGameParameter* Parameter, FText Text);

	UFUNCTION()
	void OnEnabledChangedHandler(UGameParameter* Parameter, bool bIsParameterEnabled);
};
