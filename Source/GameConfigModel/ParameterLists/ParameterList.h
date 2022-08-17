// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameConfigModel/Parameters/GameParameter.h"
#include "GameConfigModel/ParameterSources/GameParameterSourceDynamic.h"
#include "UObject/Object.h"
#include "ParameterList.generated.h"

#define GET_GAME_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)                                                                                                                                        \
	MakeShared<FGameParameterSourceDynamic>(                                                                                                                                                           \
		TArray<FString>({GET_FUNCTION_NAME_STRING_CHECKED(UGameParameter, GetGameUserSettings), GET_FUNCTION_NAME_STRING_CHECKED(UGameUserSettings, FunctionOrPropertyName)}))

/**
 * List of game parameters
 */
UCLASS()
class GAMECONFIGMODEL_API UParameterList : public UObject
{
	GENERATED_BODY()

public:
	UParameterList();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnParameterCreatedHandler, UGameParameter*, Parameter);

	/* Event raised after parameter creation and before initialization */
	UPROPERTY(BlueprintAssignable)
	FOnParameterCreatedHandler OnParameterCreated;

	UFUNCTION(BlueprintCallable)
	void InitializeList(APlayerController* PlayerController);

protected:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer;

	/* Create parameters */
	virtual void Initialize(ULocalPlayer* InLocalPlayer) PURE_VIRTUAL(, );

	/* Creates new parameter. Returns nullptr if name not unique in list */
	UGameParameter* CreateParameter(const FName& UniqueName, TSubclassOf<UGameParameter> Class);

private:
	UPROPERTY(Transient)
	TArray<UGameParameter*> Parameters;
};
