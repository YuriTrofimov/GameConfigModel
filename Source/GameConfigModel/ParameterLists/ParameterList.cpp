// Fill out your copyright notice in the Description page of Project Settings.

#include "ParameterList.h"

UParameterList::UParameterList() {}

void UParameterList::InitializeList(APlayerController* PlayerController)
{
	Initialize(PlayerController->GetLocalPlayer());
}

UGameParameter* UParameterList::CreateParameter(const FName& UniqueName, TSubclassOf<UGameParameter> Class)
{
	if (!LocalPlayer) return nullptr;

	ensureMsgf(Class, TEXT("Error! Parameter class is null!"));
	if (!Class) return nullptr;

	for (const auto* Param : Parameters)
	{
		const bool bExists = Param->GetUniqueName() == UniqueName;
		ensureMsgf(!bExists, TEXT("Error! Parameter name:%s is not unique!"), *UniqueName.ToString());
		if (bExists)
		{
			return nullptr;
		}
	}

	auto* Result = NewObject<UGameParameter>(this, Class);
	if (!Result) return nullptr;

	Parameters.Add(Result);
	Result->SetUniqueName(UniqueName);
	OnParameterCreated.Broadcast(Result);
	return Result;
}
