// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#pragma once

#include "CoreMinimal.h"
#include "GameConfigModel/Parameters/ParameterLookup.h"
#include "UObject/Object.h"
#include "ParameterLookup_Resolution.generated.h"

/**
 * Screen resolution lookup parameter
 */
UCLASS()
class GAMECONFIGMODEL_API UParameterLookup_Resolution : public UParameterLookup_Enum
{
	GENERATED_BODY()

public:
	UParameterLookup_Resolution();

protected:
	virtual void BeginInitialize() override;
	virtual void OnInitialized() override;
	virtual void SaveBaseValue() override;
	virtual void ResetToDefault() override;
	virtual void LoadBaseValue() override;
	virtual void SetValueFromString(FString InStringValue, EGameParameterChangeReason Reason) override;
	void InitializeResolutions();
	static bool ShouldAllowFullScreenResolution(const FScreenResolutionRHI& SrcScreenRes, int32 FilterThreshold);
	static void GetStandardWindowResolutions(const FIntPoint& MinResolution, const FIntPoint& MaxResolution, float MinAspectRatio, TArray<FIntPoint>& OutResolutions);
	void RefreshOptions(EWindowMode::Type WindowMode);
	void SelectAppropriateResolutions();
	void SelectCurrentResolution();
	/* Returns index of resolution */
	int32 FindDisplayResolutionIndex(const FIntPoint& InPoint) const;
	/* Tries to find resolution index. Returns last resolution index if current not exists*/
	int32 FindValidDisplayResolutionIndex(const FIntPoint& InPoint) const;
	/* Return index of closest resolution to provided dimensions */
	int32 FindClosestDisplayResolutionIndex(const FIntPoint& InResolution) const;

	TOptional<EWindowMode::Type> LastWindowMode;

	struct FScreenResolutionEntry
	{
		uint32 Width = 0;
		uint32 Height = 0;
		uint32 RefreshRate = 0;
		FText OverrideText;

		FIntPoint GetResolution() const { return FIntPoint(Width, Height); }
		FText GetDisplayText() const;
	};

	/** An array of strings the map to resolutions, populated based on the window mode */
	TArray<TSharedPtr<FScreenResolutionEntry>> Resolutions;

	/** An array of strings the map to fullscreen resolutions */
	TArray<TSharedPtr<FScreenResolutionEntry>> ResolutionsFullscreen;

	/** An array of strings the map to windowed fullscreen resolutions */
	TArray<TSharedPtr<FScreenResolutionEntry>> ResolutionsWindowedFullscreen;

	/** An array of strings the map to windowed resolutions */
	TArray<TSharedPtr<FScreenResolutionEntry>> ResolutionsWindowed;
};
