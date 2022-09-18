// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "VideoQualityParameterList.h"
#include "GameConfigModel/Parameters/ParameterLookup.h"
#include "GameConfigModel/Parameters/Video/ParameterLookup_GraphicsQualityPreset.h"
#include "GameFramework/GameUserSettings.h"

#define LOCTEXT_NAMESPACE "GameParameters"

UVideoQualityParameterList::UVideoQualityParameterList() {}

void UVideoQualityParameterList::ResetToDefault()
{
	if (!PresetParameter) return;

	auto* UserSettings = GEngine->GetGameUserSettings();
	UserSettings->RunHardwareBenchmark();
	SetQualityLevels(UserSettings->ScalabilityQuality);
	PresetParameter->ResetToDefault();
}

void UVideoQualityParameterList::Initialize(ULocalPlayer* InLocalPlayer)
{
	if (LocalPlayer == InLocalPlayer) return;
	LocalPlayer = InLocalPlayer;

	PresetParameter = CreateGraphicsPreset();
	CreateGlobalIlluminationQuality(PresetParameter);
	CreateShadowsQuality(PresetParameter);
	CreateAntiAliasingQuality(PresetParameter);
	CreateViewDistance(PresetParameter);
	CreateTextureQuality(PresetParameter);
	CreateVisualEffectQuality(PresetParameter);
	CreateReflectionQuality(PresetParameter);
	CreatePostProcessingQuality(PresetParameter);
}

UGameParameter* UVideoQualityParameterList::CreateGraphicsPreset()
{
	auto* Param = Cast<UParameteLookup_GraphicsQualityPreset>(CreateParameter(TEXT("GraphicsQualityPreset"), UParameteLookup_GraphicsQualityPreset::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("GraphicsQualityPreset_Name", "Quality Presets"));
	Param->SetDescription(LOCTEXT(
		"GraphicsQualityPreset_Description", "Quality Preset allows you to adjust multiple video options at once. Try a few options to see what fits your preference and device's performance."));
	Param->Initialize(LocalPlayer);
	return Param;
}

UGameParameter* UVideoQualityParameterList::CreateGlobalIlluminationQuality(UGameParameter* InQualityPresetParameter)
{
	auto* Param = Cast<UParameterLookup_Numeric>(CreateParameter(TEXT("GlobalIlluminationQuality"), UParameterLookup_Numeric::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("GlobalIlluminationQuality_Name", "Global Illumination"));
	Param->SetDescription(
		LOCTEXT("GlobalIlluminationQuality_Description", "Global Illumination controls the quality of dynamically calculated indirect lighting bounces, sky shadowing and Ambient Occlusion. Settings "
														 "of 'High' and above use more accurate ray tracing methods to solve lighting, but can reduce performance."));
	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(GetGlobalIlluminationQuality));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetGlobalIlluminationQuality));
	Param->SetApplyOnSelectionChanged(true);
	Param->AddNumericOption(0, LOCTEXT("VisualEffectQualityLow", "Low"));
	Param->AddNumericOption(1, LOCTEXT("VisualEffectQualityMedium", "Medium"));
	Param->AddNumericOption(2, LOCTEXT("VisualEffectQualityHigh", "High"));
	Param->AddNumericOption(3, LOCTEXT("VisualEffectQualityEpic", "Epic"));
	Param->SetParentParameter(InQualityPresetParameter);
	Param->Initialize(LocalPlayer);
	return Param;
}

UGameParameter* UVideoQualityParameterList::CreateShadowsQuality(UGameParameter* InQualityPresetParameter)
{
	auto* Param = Cast<UParameterLookup_Numeric>(CreateParameter(TEXT("ShadowsQuality"), UParameterLookup_Numeric::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("ShadowsQuality_Name", "Shadows"));
	Param->SetDescription(LOCTEXT("ShadowsQuality_Description",
		"Shadow quality determines the resolution and view distance of dynamic shadows. Shadows improve visual quality and give better depth perception, but can reduce performance."));
	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(GetShadowQuality));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetShadowQuality));
	Param->SetApplyOnSelectionChanged(true);
	Param->AddNumericOption(0, LOCTEXT("ShadowsQualityLow", "Off"));
	Param->AddNumericOption(1, LOCTEXT("ShadowsQualityMedium", "Medium"));
	Param->AddNumericOption(2, LOCTEXT("ShadowsQualityHigh", "High"));
	Param->AddNumericOption(3, LOCTEXT("ShadowsQualityEpic", "Epic"));
	Param->SetParentParameter(InQualityPresetParameter);
	Param->Initialize(LocalPlayer);
	return Param;
}

UGameParameter* UVideoQualityParameterList::CreateAntiAliasingQuality(UGameParameter* InQualityPresetParameter)
{
	auto* Param = Cast<UParameterLookup_Numeric>(CreateParameter(TEXT("AntiAliasingQuality"), UParameterLookup_Numeric::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("AntiAliasingQuality_Name", "Anti-Aliasing"));
	Param->SetDescription(LOCTEXT("AntiAliasingQuality_Description",
		"Anti-Aliasing reduces jaggy artifacts along geometry edges. Increasing this setting will make edges look smoother, but can reduce performance. Higher settings mean more anti-aliasing."));
	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(GetAntiAliasingQuality));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetAntiAliasingQuality));
	Param->SetApplyOnSelectionChanged(true);
	Param->AddNumericOption(0, LOCTEXT("AntiAliasingQualityLow", "Off"));
	Param->AddNumericOption(1, LOCTEXT("AntiAliasingQualityMedium", "Medium"));
	Param->AddNumericOption(2, LOCTEXT("AntiAliasingQualityHigh", "High"));
	Param->AddNumericOption(3, LOCTEXT("AntiAliasingQualityEpic", "Epic"));
	Param->SetParentParameter(InQualityPresetParameter);
	Param->Initialize(LocalPlayer);
	return Param;
}

UGameParameter* UVideoQualityParameterList::CreateViewDistance(UGameParameter* InQualityPresetParameter)
{
	auto* Param = Cast<UParameterLookup_Numeric>(CreateParameter(TEXT("ViewDistance"), UParameterLookup_Numeric::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("ViewDistance_Name", "View Distance"));
	Param->SetDescription(LOCTEXT("ViewDistance_Description", "View distance determines how far away objects are culled for performance."));
	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(GetViewDistanceQuality));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetViewDistanceQuality));
	Param->SetApplyOnSelectionChanged(true);
	Param->AddNumericOption(0, LOCTEXT("ViewDistanceNear", "Near"));
	Param->AddNumericOption(1, LOCTEXT("ViewDistanceMedium", "Medium"));
	Param->AddNumericOption(2, LOCTEXT("ViewDistanceFar", "Far"));
	Param->AddNumericOption(3, LOCTEXT("ViewDistanceEpic", "Epic"));
	Param->SetParentParameter(InQualityPresetParameter);
	Param->Initialize(LocalPlayer);
	return Param;
}

UGameParameter* UVideoQualityParameterList::CreateTextureQuality(UGameParameter* InQualityPresetParameter)
{
	auto* Param = Cast<UParameterLookup_Numeric>(CreateParameter(TEXT("TextureQuality"), UParameterLookup_Numeric::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("TextureQuality_Name", "Textures"));
	Param->SetDescription(
		LOCTEXT("TextureQuality_Description", "Texture quality determines the resolution of textures in game. Increasing this setting will make objects more detailed, but can reduce performance."));
	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(GetTextureQuality));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetTextureQuality));
	Param->SetApplyOnSelectionChanged(true);
	Param->AddNumericOption(0, LOCTEXT("TextureQualityLow", "Low"));
	Param->AddNumericOption(1, LOCTEXT("TextureQualityMedium", "Medium"));
	Param->AddNumericOption(2, LOCTEXT("TextureQualityHigh", "High"));
	Param->AddNumericOption(3, LOCTEXT("TextureQualityEpic", "Epic"));
	Param->SetParentParameter(InQualityPresetParameter);
	Param->Initialize(LocalPlayer);
	return Param;
}

UGameParameter* UVideoQualityParameterList::CreateVisualEffectQuality(UGameParameter* InQualityPresetParameter)
{
	auto* Param = Cast<UParameterLookup_Numeric>(CreateParameter(TEXT("VisualEffectQuality"), UParameterLookup_Numeric::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("VisualEffectQuality_Name", "Effects"));
	Param->SetDescription(LOCTEXT("VisualEffectQuality_Description",
		"Effects determines the quality of visual effects and lighting in game. Increasing this setting will increase the quality of visual effects, but can reduce performance."));
	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(GetVisualEffectQuality));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetVisualEffectQuality));
	Param->SetApplyOnSelectionChanged(true);
	Param->AddNumericOption(0, LOCTEXT("VisualEffectQualityLow", "Low"));
	Param->AddNumericOption(1, LOCTEXT("VisualEffectQualityMedium", "Medium"));
	Param->AddNumericOption(2, LOCTEXT("VisualEffectQualityHigh", "High"));
	Param->AddNumericOption(3, LOCTEXT("VisualEffectQualityEpic", "Epic"));
	Param->SetParentParameter(InQualityPresetParameter);
	Param->Initialize(LocalPlayer);
	return Param;
}

UGameParameter* UVideoQualityParameterList::CreateReflectionQuality(UGameParameter* InQualityPresetParameter)
{
	auto* Param = Cast<UParameterLookup_Numeric>(CreateParameter(TEXT("ReflectionQuality"), UParameterLookup_Numeric::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("ReflectionQuality_Name", "Reflections"));
	Param->SetDescription(LOCTEXT("ReflectionQuality_Description", "Reflection quality determines the resolution and accuracy of reflections.  Settings of 'High' and above use more accurate ray "
																   "tracing methods to solve reflections, but can reduce performance."));
	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(GetReflectionQuality));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetReflectionQuality));
	Param->SetApplyOnSelectionChanged(true);
	Param->AddNumericOption(0, LOCTEXT("VisualEffectQualityLow", "Low"));
	Param->AddNumericOption(1, LOCTEXT("VisualEffectQualityMedium", "Medium"));
	Param->AddNumericOption(2, LOCTEXT("VisualEffectQualityHigh", "High"));
	Param->AddNumericOption(3, LOCTEXT("VisualEffectQualityEpic", "Epic"));
	Param->SetParentParameter(InQualityPresetParameter);
	Param->Initialize(LocalPlayer);
	return Param;
}

UGameParameter* UVideoQualityParameterList::CreatePostProcessingQuality(UGameParameter* InQualityPresetParameter)
{
	auto* Param = Cast<UParameterLookup_Numeric>(CreateParameter(TEXT("PostProcessingQuality"), UParameterLookup_Numeric::StaticClass()));
	if (!Param) return nullptr;
	Param->SetDisplayName(LOCTEXT("PostProcessingQuality_Name", "Post Processing"));
	Param->SetDescription(LOCTEXT("PostProcessingQuality_Description",
		"Post Processing effects include Motion Blur, Depth of Field and Bloom. Increasing this setting improves the quality of post process effects, but can reduce performance."));
	Param->SetDynamicGetter(GET_GAME_SETTINGS_FUNCTION_PATH(GetPostProcessingQuality));
	Param->SetDynamicSetter(GET_GAME_SETTINGS_FUNCTION_PATH(SetPostProcessingQuality));
	Param->SetApplyOnSelectionChanged(true);
	Param->AddNumericOption(0, LOCTEXT("PostProcessingQualityLow", "Low"));
	Param->AddNumericOption(1, LOCTEXT("PostProcessingQualityMedium", "Medium"));
	Param->AddNumericOption(2, LOCTEXT("PostProcessingQualityHigh", "High"));
	Param->AddNumericOption(3, LOCTEXT("PostProcessingQualityEpic", "Epic"));
	Param->SetParentParameter(InQualityPresetParameter);
	Param->Initialize(LocalPlayer);
	return Param;
}

#undef LOCTEXT_NAMESPACE
