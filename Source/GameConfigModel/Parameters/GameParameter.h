// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameParameter.generated.h"

class FParameterEditCondition;

/**
 * Game parameter
 */
UCLASS()
class GAMECONFIGMODEL_API UGameParameter : public UObject
{
	GENERATED_BODY()

public:
	UGameParameter();

public:
	DECLARE_EVENT_OneParam(UGameParameter, FOnParameterChanged, UGameParameter*);
	DECLARE_EVENT_OneParam(UGameParameter, FOnParameterEditConditionChanged, UGameParameter*);
	FOnParameterChanged OnParameterChangedEvent;
	FOnParameterEditConditionChanged OnParameterEditConditionChangedEvent;
protected:
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
	/* Parameter edit conditions */
	UPROPERTY(Transient)
	TArray<TSharedRef<FParameterEditCondition>> EditConditions;
public:
#pragma region GettersSetters
	UFUNCTION(BlueprintPure, Category="GameConfig|Parameters")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; }

	void SetDisplayName(const FText& InDisplayName) { DisplayName = InDisplayName; }

	UFUNCTION(BlueprintPure, Category="GameConfig|Parameters")
	FORCEINLINE FName GetUniqueName() const { return UniqueName; }

	void SetName(const FName& InName) { UniqueName = InName; }

	UFUNCTION(BlueprintPure, Category="GameConfig|Parameters")
	FORCEINLINE FText GetDescription() const { return Description; }

	void SetDescription(const FText& InDescription) { Description = InDescription; }

	UFUNCTION(BlueprintPure, Category="GameConfig|Parameters")
	const FORCEINLINE FGameplayTagContainer& GetTags() const { return Tags; }

	void AddTag(const FGameplayTag& InTag) { Tags.AddTag(InTag); }

	void SetParentParameter(UGameParameter* InParent);
	UGameParameter* GetParentParameter() const { return ParentParameter; }
#pragma endregion

	/** Initialize parameter using owning local player */
	void Initialize(ULocalPlayer* InLocalPlayer);

	/* Add parameter edit condition */
	void AddEditCondition(const TSharedRef<FParameterEditCondition>& InEditCondition);

	/* Returns TRUE if parameter can be edited */
	UFUNCTION(BlueprintPure, Category="GameConfig|Parameters|Conditions")
	bool CanEdit();
protected:
	/* Child edit condition changed event handler */
	virtual void OnEditConditionChangedHandler();
};
