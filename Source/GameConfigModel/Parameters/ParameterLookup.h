// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "GameParameter.h"
#include "GameConfigModel/ParameterSources/GameParameterSource.h"
#include "ParameterLookup.generated.h"

USTRUCT(BlueprintType)
struct FParameterLookupOption
{
	GENERATED_BODY()

	FParameterLookupOption() = default;

	FParameterLookupOption(const FString& InValue, const FText& InCaption)
		: Value(InValue), Caption(InCaption)
	{
	}

	/* Parameter value */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Value;

	/* Parameter value display caption */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Caption;
};

/**
 * Parameter wich value is picked from the list
 */
UCLASS(Abstract, BlueprintType, NotBlueprintable)
class GAMECONFIGMODEL_API UParameterLookup : public UGameParameter
{
	GENERATED_BODY()

public:
	UParameterLookup();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLookupSelectionChangedHandler, FParameterLookupOption, Option, int32, SelectedIndex, bool, bCanBack, bool, bCanForward);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLookupOptionsListChangedHandler);

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnLookupSelectionChangedHandler OnSelectionChanged;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnLookupOptionsListChangedHandler OnOptionsListChanged;

	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void SelectOptionByIndex(int32 OptionIndex);
	void SelectOptionByIndex(int32 OptionIndex, EGameParameterChangeReason Reason);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SelectNextOption();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SelectPreviousOption();

	UFUNCTION(BlueprintPure, Category = "UI")
	virtual int32 GetSelectedIndex();

	UFUNCTION(BlueprintPure, Category = "UI")
	virtual void GetOptionsCaptions(TArray<FText>& Captions);

	void AddOption(const FString& Value, const FText& Caption);
	void RemoveOption(const FString& InValue);
	void GetOptionsValues(TArray<FString>& Values);

	/* Returns TRUE if option with current value exists */
	bool HasOptionValue(const FString& InOptionValue);
	void SetDynamicGetter(const TSharedRef<FGameParameterSource>& InParameterGetter);
	void SetDynamicSetter(const TSharedRef<FGameParameterSource>& InParameterSetter);

	FString GetValueAsString();
	void SetValueFromString(FString InStringValue);
	void SetDefaultValueFromString(FString InStringValue);

	/* Reset value to default with Dynamic Setter */
	virtual void ResetToDefault() override;
	/* Load BaseValue variable via Dynamic Getter */
	virtual void LoadBaseValue() override;
	/* Use BaseValue variable with Dynamic Setter */
	virtual void SaveBaseValue() override;
	virtual void BeginInitialize() override;

	/* Toggle apply parameter value on selected option changed event */
	void SetApplyOnSelectionChanged(bool bInEnabled)
	{
		bApplyOnSelectionChanged = bInEnabled;
	}

protected:
	virtual void SelectOptionByValue(const FString& InValue, EGameParameterChangeReason Reason);
	virtual void SetValueFromString(FString InStringValue, EGameParameterChangeReason Reason);
	virtual void OnInitialized() override;
	virtual void OnParentParameterChangedHandler(UGameParameter* InParameter, EGameParameterChangeReason InChangeReason) override;
	void RaiseOptionsListChanged() const;

	TSharedPtr<FGameParameterSource> ParameterGetter;
	TSharedPtr<FGameParameterSource> ParameterSetter;
	/* Parameter default value */
	TOptional<FString> DefaultValue;
	/* Parameter base value */
	FString BaseValue;
	/* List of parameter values */
	TArray<FParameterLookupOption> Options;
	/* Selected option index */
	int32 SelectedIndex = 0;
	/* If true - parameter will be applied on if selected option changed */
	bool bApplyOnSelectionChanged = false;

private:
	void OnGetterReady();
	void OnSetterReady();
};

/*
 * Enum lookup parameter
 */
UCLASS()
class GAMECONFIGMODEL_API UParameterLookup_Enum : public UParameterLookup
{
	GENERATED_BODY()

public:
	UParameterLookup_Enum();

	template <typename EnumType>
	void SetDefaultValue(EnumType InEnumValue)
	{
		const FString StringValue = StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(InEnumValue));
		SetDefaultValueFromString(StringValue);
	}

	template <typename EnumType>
	void AddEnumOption(EnumType InEnumValue, const FText& InOptionText)
	{
		const FString StringValue = StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(InEnumValue));
		AddOption(StringValue, InOptionText);
	}

	template <typename EnumType>
	EnumType GetValue()
	{
		const FString Value = GetValueAsString();
		return static_cast<EnumType>(StaticEnum<EnumType>()->GetValueByNameString(Value));
	}

	template <typename EnumType>
	void SetValue(EnumType InEnumValue)
	{
		const FString StringValue = StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(InEnumValue));
		SetValueFromString(StringValue);
	}

protected:
	virtual void OnInitialized() override;
};

/*
 * Numeric lookup parameter
 */
UCLASS()
class GAMECONFIGMODEL_API UParameterLookup_Numeric : public UParameterLookup
{
	GENERATED_BODY()

public:
	UParameterLookup_Numeric();

	template <typename NumberType>
	void SetDefaultValue(NumberType InValue)
	{
		SetDefaultValueFromString(LexToString(InValue));
	}

	template <typename NumberType>
	void AddNumericOption(NumberType InValue, const FText& InOptionText)
	{
		AddOption(LexToString(InValue), InOptionText);
	}

	template <typename NumberType>
	NumberType GetValue() const
	{
		const FString ValueString = GetValueAsString();
		NumberType OutValue;
		LexFromString(OutValue, *ValueString);
		return OutValue;
	}

	template <typename NumberType>
	void SetValue(NumberType InValue)
	{
		SetValueFromString(LexToString(InValue));
	}

protected:
	virtual void OnInitialized() override;
};

/*
 * Boolean lookup parameter
 */
UCLASS()
class GAMECONFIGMODEL_API UParameterLookup_Bool : public UParameterLookup
{
	GENERATED_BODY()

public:
	UParameterLookup_Bool();

	UFUNCTION(BlueprintCallable)
	void SetValue(bool bNewValue = false);

	void SetDefaultValue(bool Value);
	void SetTrueText(const FText& InText);
	void SetFalseText(const FText& InText);

#if !UE_BUILD_SHIPPING
	void SetTrueText(const FString& Value) { SetTrueText(FText::FromString(Value)); }
	void SetFalseText(const FString& Value) { SetFalseText(FText::FromString(Value)); }
#endif
};
