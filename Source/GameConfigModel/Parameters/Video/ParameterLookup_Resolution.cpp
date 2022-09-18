// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "ParameterLookup_Resolution.h"
#include "GameFramework/GameUserSettings.h"

#define LOCTEXT_NAMESPACE "VideoSettings"

UParameterLookup_Resolution::UParameterLookup_Resolution()
{
	Type = EParameterType::Custom;
}

void UParameterLookup_Resolution::BeginInitialize()
{
	EndInitialize();
}

void UParameterLookup_Resolution::OnInitialized()
{
	InitializeResolutions();
	SelectCurrentResolution();
}

void UParameterLookup_Resolution::SaveBaseValue()
{
	if (Resolutions.IsValidIndex(SelectedIndex) && Resolutions[SelectedIndex].IsValid())
	{
		GEngine->GetGameUserSettings()->SetScreenResolution(Resolutions[SelectedIndex]->GetResolution());
	}
}

void UParameterLookup_Resolution::ResetToDefault()
{
	if (bEnabled && bVisible)
	{
		SelectCurrentResolution();
	}
}

void UParameterLookup_Resolution::LoadBaseValue()
{
	SelectCurrentResolution();
}

void UParameterLookup_Resolution::SetValueFromString(FString InStringValue, EGameParameterChangeReason Reason) {}

void UParameterLookup_Resolution::InitializeResolutions()
{
	Resolutions.Empty();
	ResolutionsFullscreen.Empty();
	ResolutionsWindowed.Empty();
	ResolutionsWindowedFullscreen.Empty();

	FDisplayMetrics InitialDisplayMetrics;
	FSlateApplication::Get().GetInitialDisplayMetrics(InitialDisplayMetrics);

	FScreenResolutionArray ResArray;
	RHIGetAvailableResolutions(ResArray, true);

	// Determine available windowed modes
	{
		TArray<FIntPoint> WindowedResolutions;
		const FIntPoint MinResolution(1280, 720);
		// Use the primary display resolution minus 1 to exclude the primary display resolution from the list.
		// This is so you don't make a window so large that part of the game is off screen and you are unable to change resolutions back.
		const FIntPoint MaxResolution(InitialDisplayMetrics.PrimaryDisplayWidth - 1, InitialDisplayMetrics.PrimaryDisplayHeight - 1);
		// Excluding 4:3 and below
		const float MinAspectRatio = 16.0f / 10.f;

		if (MaxResolution.X >= MinResolution.X && MaxResolution.Y >= MinResolution.Y)
		{
			GetStandardWindowResolutions(MinResolution, MaxResolution, MinAspectRatio, WindowedResolutions);
		}

		if (GSystemResolution.WindowMode == EWindowMode::Windowed)
		{
			WindowedResolutions.AddUnique(FIntPoint(GSystemResolution.ResX, GSystemResolution.ResY));
			WindowedResolutions.Sort([](const FIntPoint& A, const FIntPoint& B) { return A.X != B.X ? A.X < B.X : A.Y < B.Y; });
		}

		// If there were no standard resolutions. Add the primary display size, just so one exists.
		// This might happen if we are running on a non-standard device.
		if (WindowedResolutions.Num() == 0)
		{
			WindowedResolutions.Add(FIntPoint(InitialDisplayMetrics.PrimaryDisplayWidth, InitialDisplayMetrics.PrimaryDisplayHeight));
		}

		ResolutionsWindowed.Empty(WindowedResolutions.Num());
		for (const FIntPoint& Res : WindowedResolutions)
		{
			TSharedRef<FScreenResolutionEntry> Entry = MakeShared<FScreenResolutionEntry>();
			Entry->Width = Res.X;
			Entry->Height = Res.Y;

			ResolutionsWindowed.Add(Entry);
		}
	}

	// Determine available windowed full-screen modes
	{
		const FScreenResolutionRHI* RHIInitialResolution = ResArray.FindByPredicate([InitialDisplayMetrics](const FScreenResolutionRHI& ScreenRes) {
			return ScreenRes.Width == InitialDisplayMetrics.PrimaryDisplayWidth && ScreenRes.Height == InitialDisplayMetrics.PrimaryDisplayHeight;
		});

		const TSharedRef<FScreenResolutionEntry> Entry = MakeShared<FScreenResolutionEntry>();
		if (RHIInitialResolution)
		{
			// If this is in the official list use that
			Entry->Width = RHIInitialResolution->Width;
			Entry->Height = RHIInitialResolution->Height;
			Entry->RefreshRate = RHIInitialResolution->RefreshRate;
		}
		else
		{
			// Custom resolution the RHI doesn't expect
			Entry->Width = InitialDisplayMetrics.PrimaryDisplayWidth;
			Entry->Height = InitialDisplayMetrics.PrimaryDisplayHeight;

			// TODO: Unsure how to calculate refresh rate
			Entry->RefreshRate = FPlatformMisc::GetMaxRefreshRate();
		}

		ResolutionsWindowedFullscreen.Add(Entry);
	}

	// Determine available full-screen modes
	if (ResArray.Num() > 0)
	{
		// try more strict first then more relaxed, we want at least one resolution to remain
		for (int32 FilterThreshold = 0; FilterThreshold < 3; ++FilterThreshold)
		{
			for (int32 ModeIndex = 0; ModeIndex < ResArray.Num(); ModeIndex++)
			{
				const FScreenResolutionRHI& ScreenRes = ResArray[ModeIndex];

				// first try with struct test, than relaxed test
				if (ShouldAllowFullScreenResolution(ScreenRes, FilterThreshold))
				{
					TSharedRef<FScreenResolutionEntry> Entry = MakeShared<FScreenResolutionEntry>();
					Entry->Width = ScreenRes.Width;
					Entry->Height = ScreenRes.Height;
					Entry->RefreshRate = ScreenRes.RefreshRate;

					ResolutionsFullscreen.Add(Entry);
				}
			}

			if (ResolutionsFullscreen.Num())
			{
				// we found some resolutions, otherwise we try with more relaxed tests
				break;
			}
		}
	}

	SelectAppropriateResolutions();
}

bool UParameterLookup_Resolution::ShouldAllowFullScreenResolution(const FScreenResolutionRHI& SrcScreenRes, int32 FilterThreshold)
{
	FScreenResolutionRHI ScreenRes = SrcScreenRes;

	// expected: 4:3=1.333, 16:9=1.777, 16:10=1.6, multi-monitor-wide: >2
	const bool bIsPortrait = ScreenRes.Width < ScreenRes.Height;
	float AspectRatio = static_cast<float>(ScreenRes.Width) / static_cast<float>(ScreenRes.Height);

	// If portrait, flip values back to landscape so we can don't have to special case all the tests below
	if (bIsPortrait)
	{
		AspectRatio = 1.0f / AspectRatio;
		ScreenRes.Width = SrcScreenRes.Height;
		ScreenRes.Height = SrcScreenRes.Width;
	}

	// Filter out resolutions that don't match the native aspect ratio of the primary monitor
	// TODO: Other games allow the user to choose which monitor the games goes fullscreen on. This would allow
	// this filtering to be correct when the users monitors are of different types! ATM, the game can change
	// which monitor it uses based on other factors (max window overlap etc.) so we could end up choosing a
	// resolution which the target monitor doesn't support.
	if (FilterThreshold < 1)
	{
		FDisplayMetrics DisplayMetrics;
		FSlateApplication::Get().GetInitialDisplayMetrics(DisplayMetrics);

		// Default display aspect to required aspect in case this platform can't provide the information. Forces acceptance of this resolution.
		float DisplayAspect = AspectRatio;

		// Some platforms might not be able to detect the native resolution of the display device, so don't filter in that case
		for (int32 MonitorIndex = 0; MonitorIndex < DisplayMetrics.MonitorInfo.Num(); ++MonitorIndex)
		{
			const FMonitorInfo& MonitorInfo = DisplayMetrics.MonitorInfo[MonitorIndex];

			if (MonitorInfo.bIsPrimary)
			{
				DisplayAspect = static_cast<float>(MonitorInfo.NativeWidth) / static_cast<float>(MonitorInfo.NativeHeight);
				break;
			}
		}

		// If aspects are not almost exactly equal, reject
		if (FMath::Abs(DisplayAspect - AspectRatio) > KINDA_SMALL_NUMBER)
		{
			return false;
		}
	}

	// more relaxed tests have a larger FilterThreshold

	// minimum is 1280x720
	if (FilterThreshold < 2 && (ScreenRes.Width < 1280 || ScreenRes.Height < 720))
	{
		// filter resolutions that are too small
		return false;
	}

	return true;
}

void UParameterLookup_Resolution::GetStandardWindowResolutions(const FIntPoint& MinResolution, const FIntPoint& MaxResolution, float MinAspectRatio, TArray<FIntPoint>& OutResolutions)
{
	static TArray<FIntPoint> StandardResolutions;
	if (StandardResolutions.Num() == 0)
	{
		// Standard resolutions as provided by Wikipedia (http://en.wikipedia.org/wiki/Graphics_display_resolution)
		// Extended Graphics Array
		{
			new (StandardResolutions) FIntPoint(1024, 768);	 // XGA

			// WXGA (3 versions)
			new (StandardResolutions) FIntPoint(1366, 768);	 // FWXGA
			new (StandardResolutions) FIntPoint(1360, 768);
			new (StandardResolutions) FIntPoint(1280, 800);

			new (StandardResolutions) FIntPoint(1152, 864);	  // XGA+
			new (StandardResolutions) FIntPoint(1440, 900);	  // WXGA+
			new (StandardResolutions) FIntPoint(1280, 1024);  // SXGA
			new (StandardResolutions) FIntPoint(1400, 1050);  // SXGA+
			new (StandardResolutions) FIntPoint(1680, 1050);  // WSXGA+
			new (StandardResolutions) FIntPoint(1600, 1200);  // UXGA
			new (StandardResolutions) FIntPoint(1920, 1200);  // WUXGA
		}

		// Quad Extended Graphics Array
		{
			new (StandardResolutions) FIntPoint(2048, 1152);  // QWXGA
			new (StandardResolutions) FIntPoint(2048, 1536);  // QXGA
			new (StandardResolutions) FIntPoint(2560, 1600);  // WQXGA
			new (StandardResolutions) FIntPoint(2560, 2048);  // QSXGA
			new (StandardResolutions) FIntPoint(3200, 2048);  // WQSXGA
			new (StandardResolutions) FIntPoint(3200, 2400);  // QUXGA
			new (StandardResolutions) FIntPoint(3840, 2400);  // WQUXGA
		}

		// Hyper Extended Graphics Array
		{
			new (StandardResolutions) FIntPoint(4096, 3072);  // HXGA
			new (StandardResolutions) FIntPoint(5120, 3200);  // WHXGA
			new (StandardResolutions) FIntPoint(5120, 4096);  // HSXGA
			new (StandardResolutions) FIntPoint(6400, 4096);  // WHSXGA
			new (StandardResolutions) FIntPoint(6400, 4800);  // HUXGA
			new (StandardResolutions) FIntPoint(7680, 4800);  // WHUXGA
		}

		// High-Definition
		{
			new (StandardResolutions) FIntPoint(640, 360);	   // nHD
			new (StandardResolutions) FIntPoint(960, 540);	   // qHD
			new (StandardResolutions) FIntPoint(1280, 720);	   // HD
			new (StandardResolutions) FIntPoint(1920, 1080);   // FHD
			new (StandardResolutions) FIntPoint(2560, 1440);   // QHD
			new (StandardResolutions) FIntPoint(3200, 1800);   // WQXGA+
			new (StandardResolutions) FIntPoint(3840, 2160);   // UHD 4K
			new (StandardResolutions) FIntPoint(4096, 2160);   // Digital Cinema Initiatives 4K
			new (StandardResolutions) FIntPoint(7680, 4320);   // FUHD
			new (StandardResolutions) FIntPoint(5120, 2160);   // UHD 5K
			new (StandardResolutions) FIntPoint(5120, 2880);   // UHD+
			new (StandardResolutions) FIntPoint(15360, 8640);  // QUHD
		}

		// Sort the list by total resolution size
		StandardResolutions.Sort([](const FIntPoint& A, const FIntPoint& B) { return (A.X * A.Y) < (B.X * B.Y); });
	}

	// Return all standard resolutions that are within the size constraints
	for (const auto& Resolution : StandardResolutions)
	{
		if (Resolution.X >= MinResolution.X && Resolution.Y >= MinResolution.Y && Resolution.X <= MaxResolution.X && Resolution.Y <= MaxResolution.Y)
		{
			const float AspectRatio = static_cast<float>(Resolution.X) / static_cast<float>(Resolution.Y);
			if (AspectRatio > MinAspectRatio || FMath::IsNearlyEqual(AspectRatio, MinAspectRatio))
			{
				OutResolutions.Add(Resolution);
			}
		}
	}
}

void UParameterLookup_Resolution::RefreshOptions(EWindowMode::Type WindowMode)
{
	Options.Empty();
	for (int32 Index = 0, Num = Resolutions.Num(); Index < Num; Index++)
	{
		FText Caption = Resolutions[Index]->GetDisplayText();
		Options.Add(FParameterLookupOption(LexToString(Index), Caption));
	}
	RaiseOptionsListChanged();
}

void UParameterLookup_Resolution::SelectAppropriateResolutions()
{
	EWindowMode::Type const WindowMode = GetGameUserSettings()->GetFullscreenMode();
	if (LastWindowMode != WindowMode)
	{
		LastWindowMode = WindowMode;
		Resolutions.Empty();
		switch (WindowMode)
		{
			case EWindowMode::Windowed: Resolutions.Append(ResolutionsWindowed); break;
			case EWindowMode::WindowedFullscreen: Resolutions.Append(ResolutionsWindowedFullscreen); break;
			case EWindowMode::Fullscreen: Resolutions.Append(ResolutionsFullscreen); break;
			default: break;
		}
		RefreshOptions(WindowMode);
		RaiseEditConditionChanged();
	}
}

void UParameterLookup_Resolution::SelectCurrentResolution()
{
	const UGameUserSettings* UserSettings = GetGameUserSettings();
	const FIntPoint CurrentResolution = UserSettings->GetScreenResolution();
	SelectAppropriateResolutions();
	SelectOptionByIndex(FindClosestDisplayResolutionIndex(CurrentResolution));
}

int32 UParameterLookup_Resolution::FindDisplayResolutionIndex(const FIntPoint& InPoint) const
{
	for (int32 i = 0, Num = Resolutions.Num(); i < Num; ++i)
	{
		if (Resolutions[i]->GetResolution() == InPoint)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UParameterLookup_Resolution::FindValidDisplayResolutionIndex(const FIntPoint& InPoint) const
{
	int32 Result = FindDisplayResolutionIndex(InPoint);
	if (Result == INDEX_NONE && Resolutions.Num() > 0)
	{
		Result = Resolutions.Num() - 1;
	}
	return Result;
}

int32 UParameterLookup_Resolution::FindClosestDisplayResolutionIndex(const FIntPoint& InResolution) const
{
	int32 Index = 0;
	int32 LastDiff = InResolution.SizeSquared();
	for (int32 i = 0, Num = Resolutions.Num(); i < Num; ++i)
	{
		// Compare the squared diagonals
		const int32 Diff = FMath::Abs(InResolution.SizeSquared() - Resolutions[i]->GetResolution().SizeSquared());
		if (Diff <= LastDiff)
		{
			Index = i;
		}
		LastDiff = Diff;
	}
	return Index;
}

FText UParameterLookup_Resolution::FScreenResolutionEntry::GetDisplayText() const
{
	if (!OverrideText.IsEmpty())
	{
		return OverrideText;
	}

	FText Aspect = FText::GetEmpty();

	// expected: 4:3=1.333, 16:9=1.777, 16:10=1.6, multi-monitor-wide: >2
	const float AspectRatio = static_cast<float>(Width) / static_cast<float>(Height);

	if (FMath::Abs(AspectRatio - (4.0f / 3.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-4:3", "4:3");
	}
	else if (FMath::Abs(AspectRatio - (16.0f / 9.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-16:9", "16:9");
	}
	else if (FMath::Abs(AspectRatio - (16.0f / 10.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-16:10", "16:10");
	}
	else if (FMath::Abs(AspectRatio - (3.0f / 4.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-3:4", "3:4");
	}
	else if (FMath::Abs(AspectRatio - (9.0f / 16.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-9:16", "9:16");
	}
	else if (FMath::Abs(AspectRatio - (10.0f / 16.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-10:16", "10:16");
	}

	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.UseGrouping = false;

	FFormatNamedArguments Args;
	Args.Add(TEXT("X"), FText::AsNumber(Width, &FormattingOptions));
	Args.Add(TEXT("Y"), FText::AsNumber(Height, &FormattingOptions));
	Args.Add(TEXT("AspectRatio"), Aspect);
	Args.Add(TEXT("RefreshRate"), RefreshRate);

	return FText::Format(LOCTEXT("AspectRatio", "{X} x {Y}"), Args);
}

#undef LOCTEXT_NAMESPACE
