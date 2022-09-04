// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "DisplayParameterList.h"

#include "GameConfigModel/Parameters/LambdaEditCondition.h"
#include "GameConfigModel/Parameters/ParameterEditCondition.h"
#include "GameConfigModel/Parameters/ParameterLookup.h"
#include "GameConfigModel/Parameters/Video/ParameterLookup_Resolution.h"
#include "GameFramework/GameUserSettings.h"

#define LOCTEXT_NAMESPACE "GameParameters"

UDisplayParameterList::UDisplayParameterList()
{
}

void UDisplayParameterList::Initialize(ULocalPlayer* InLocalPlayer)
{
	if (LocalPlayer == InLocalPlayer) return;
	LocalPlayer = InLocalPlayer;

	auto* WindowModeParameter = CreateWindowModeParam();
	CreateResolutionParam(WindowModeParameter);
	CreateVSyncParam();
}

UParameterLookup_Enum* UDisplayParameterList::CreateWindowModeParam()
{
	auto* Param = Cast<UParameterLookup_Enum>(CreateParameter(TEXT("WindowMode"), UParameterLookup_Enum::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("WindowMode_Name", "Window Mode"));
	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(GetFullscreenMode));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetFullscreenMode));
	Param->AddEnumOption(EWindowMode::Fullscreen, LOCTEXT("WindowModeFullscreen", "Fullscreen"));
	Param->AddEnumOption(EWindowMode::WindowedFullscreen, LOCTEXT("WindowModeWindowedFullscreen", "Windowed Fullscreen"));
	Param->AddEnumOption(EWindowMode::Windowed, LOCTEXT("WindowModeWindowed", "Windowed"));

	EWorldType::Type WorldType = EWorldType::Game;
	if (const auto* World = LocalPlayer->GetWorld())
	{
		WorldType = World->WorldType;
	}
	if (WorldType == EWorldType::Game)
	{
		Param->SetDefaultValue(EWindowMode::Fullscreen);
	}
	else
	{
		Param->SetDefaultValue(EWindowMode::Windowed);
	}
	Param->Initialize(LocalPlayer);
	return Param;
}

void UDisplayParameterList::CreateResolutionParam(UParameterLookup_Enum* WindowModeParameter)
{
	auto* Param = Cast<UParameterLookup_Enum>(CreateParameter(TEXT("Resolution"), UParameterLookup_Resolution::StaticClass()));
	if (!Param) return;
	Param->SetDisplayName(LOCTEXT("Resolution_Name", "Resolution"));
	Param->SetParentParameter(WindowModeParameter);
	Param->AddEditCondition(MakeShared<FLambdaEditCondition>([WindowModeParameter](UGameParameter*)
	{
		return WindowModeParameter->GetValue<EWindowMode::Type>() == EWindowMode::Fullscreen;
	}));
	Param->Initialize(LocalPlayer);
}

void UDisplayParameterList::CreateVSyncParam()
{
	auto* Param = Cast<UParameterLookup_Bool>(CreateParameter(TEXT("VerticalSync"), UParameterLookup_Bool::StaticClass()));
	if (!Param) return;
	Param->SetDisplayName(LOCTEXT("VerticalSync_Name", "Vertical Sync"));
	Param->SetDescription(LOCTEXT("VerticalSync_Description",
		"Enabling Vertical Sync eliminates screen tearing by always rendering and presenting a full frame. Disabling Vertical Sync can give "
		"higher frame rate and better input response, but can result in horizontal screen tearing."));

	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(IsVSyncEnabled));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetVSyncEnabled));
	Param->SetDefaultValue(false);
	Param->Initialize(LocalPlayer);
}

#undef LOCTEXT_NAMESPACE
