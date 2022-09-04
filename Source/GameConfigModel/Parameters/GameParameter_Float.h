// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "GameParameter.h"
#include "GameParameter_Float.generated.h"

class FGameParameterSource;

/**
 * Float value type game parameter
 */
UCLASS()
class GAMECONFIGMODEL_API UGameParameter_Float : public UGameParameter
{
	GENERATED_BODY()

public:
	UGameParameter_Float();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChangedHandler, float, NewValue);

	UFUNCTION(BlueprintPure)
	float GetValue();

	UFUNCTION(BlueprintCallable)
	void SetValue(float InValue);

	void SetDynamicGetter(const TSharedRef<FGameParameterSource>& InParameterGetter);
	void SetDynamicSetter(const TSharedRef<FGameParameterSource>& InParameterSetter);
	virtual void ResetToDefault() override;
	virtual void BeginInitialize() override;
	virtual void SetDefaultValue(float InDefaultValue);
protected:
	virtual void OnInitialized() override;
	virtual void SetValue(float InValue, EGameParameterChangeReason Reason);
	TOptional<float> DefaultValue = 0.0f;
	TSharedPtr<FGameParameterSource> ParameterGetter;
	TSharedPtr<FGameParameterSource> ParameterSetter;
private:
	void OnGetterReady();
	void OnSetterReady();
};
