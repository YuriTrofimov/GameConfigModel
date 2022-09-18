// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameParameter.generated.h"

class FParameterEditCondition;

UENUM(BlueprintType)
enum class EParameterType : uint8
{
	/* List with values */
	Enum,
	/* Single number */
	Numeric,
	/* Bool value */
	Bool,
	/* Custom parameter type */
	Custom
};

/* Game parameter value change reason */
enum class EGameParameterChangeReason : uint8
{
	Change,
	DependencyChanged,
	ResetToDefault,
	RestoreToBase,
};

/**
 * Game parameter
 */
UCLASS(Abstract, BlueprintType)
class GAMECONFIGMODEL_API UGameParameter : public UObject
{
	GENERATED_BODY()

public:
	UGameParameter();

	DECLARE_EVENT_TwoParams(UGameParameter, FOnParameterChanged, UGameParameter*, EGameParameterChangeReason);

	DECLARE_EVENT_OneParam(UGameParameter, FOnParameterEditConditionChanged, UGameParameter*);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnParameterVisibleChangedHandler, UGameParameter*, Parameter, bool, bIsVisible);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnParameterEnabledChangedHandler, UGameParameter*, Parameter, bool, bIsEnabled);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTextValueChangedHandler, UGameParameter*, Parameter, FText, Text);

	DECLARE_EVENT_OneParam(UGameParameter, FOnParameterInitialized, UGameParameter*);

	FOnParameterChanged OnParameterChangedEvent;
	FOnParameterEditConditionChanged OnParameterEditConditionChangedEvent;
	FOnParameterInitialized OnParameterInitialized;

	UPROPERTY(BlueprintAssignable)
	FOnParameterVisibleChangedHandler OnVisibleChanged;
	UPROPERTY(BlueprintAssignable)
	FOnParameterEnabledChangedHandler OnEnabledChanged;
	UPROPERTY(BlueprintAssignable)
	FOnTextValueChangedHandler OnDisplayNameChanged;
	UPROPERTY(BlueprintAssignable)
	FOnTextValueChangedHandler OnDescriptionChanged;

protected:
	/* Parameter type */
	EParameterType Type = EParameterType::Custom;
	/* Parameter unique name */
	FName UniqueName;
	/* Parameter display name*/
	FText DisplayName;
	/* Parameter description */
	FText Description;
	/* Parameter tags */
	FGameplayTagContainer Tags;

	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer;
	/* The parent parameter */
	UPROPERTY(Transient)
	TObjectPtr<UGameParameter> ParentParameter;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UGameParameter>> ChildrenParameters;
	/* Parameter edit conditions */
	TArray<TSharedRef<FParameterEditCondition>> EditConditions;

public:
#pragma region GettersSetters
	/* Returns parameter type */
	UFUNCTION(BlueprintPure, Category = "GameConfig|Parameters")
	FORCEINLINE EParameterType GetType() const { return Type; }

	UFUNCTION(BlueprintPure, Category = "GameConfig|Parameters")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; }

	void SetDisplayName(const FText& InDisplayName);

	UFUNCTION(BlueprintPure, Category = "GameConfig|Parameters")
	FORCEINLINE FName GetUniqueName() const { return UniqueName; }

	void SetName(const FName& InName) { UniqueName = InName; }

	UFUNCTION(BlueprintPure, Category = "GameConfig|Parameters")
	FORCEINLINE FText GetDescription() const { return Description; }

	void SetDescription(const FText& InDescription);

	UFUNCTION(BlueprintPure, Category = "GameConfig|Parameters")
	const FORCEINLINE FGameplayTagContainer& GetTags() const { return Tags; }

	void AddTag(const FGameplayTag& InTag) { Tags.AddTag(InTag); }

	virtual void OnParentParameterChangedHandler(UGameParameter* InParameter, EGameParameterChangeReason InChangeReason);
	virtual void OnChildParameterChangedHandler(UGameParameter* InParameter, EGameParameterChangeReason InChangeReason);
	void SetParentParameter(UGameParameter* InParent);
	UGameParameter* GetParentParameter() const { return ParentParameter; }

	void SetUniqueName(const FName& InUniqueName) { UniqueName = InUniqueName; };
#pragma endregion

	/* Returns TRUE if parameter visible in UI */
	UFUNCTION(BlueprintPure, Category = "GameConfig|Parameters")
	virtual void IsVisible(bool& Visible);

	UFUNCTION(BlueprintPure, Category = "GameConfig|Parameters")
	virtual void IsEnabled(bool& Enabled);

	/** Initialize parameter using owning local player */
	void Initialize(ULocalPlayer* InLocalPlayer);

	/* Add parameter edit condition */
	void AddEditCondition(const TSharedRef<FParameterEditCondition>& InEditCondition);

	/* Returns TRUE if parameter can be edited */
	UFUNCTION(BlueprintPure, Category = "GameConfig|Parameters|Conditions")
	bool CanEdit();

	/* Save parameter base value */
	virtual void SaveBaseValue() PURE_VIRTUAL(, );

	/* Resets parameter value to the default */
	virtual void ResetToDefault() PURE_VIRTUAL(, );

	/* Load parameter value before any changes */
	virtual void LoadBaseValue() PURE_VIRTUAL(, );

	UFUNCTION()
	UGameUserSettings* GetGameUserSettings() const;
	void AddChildParameter(UGameParameter* InChildParameter);

protected:
	/* True - If parameter must be visible in UI */
	bool bVisible = true;

	/* True - If parameter must be enabled to edit in UI */
	bool bEnabled = true;

	/* Override this function for custom async initialization */
	virtual void BeginInitialize();
	/* Call this function after async initialization completed */
	void EndInitialize();

	/* Called when parameter initialization completed */
	virtual void OnInitialized();
	/* Child edit condition changed event handler */
	virtual void OnEditConditionChangedHandler();

	/* Notify all listeners about parameter value change */
	void RaiseParameterChanged(EGameParameterChangeReason ChangeReason);
	void RaiseEditConditionChanged();

	/** Most settings are immediately ready, but some may require startup time before it's safe to call their functions. */
	bool bReady = false;
};
