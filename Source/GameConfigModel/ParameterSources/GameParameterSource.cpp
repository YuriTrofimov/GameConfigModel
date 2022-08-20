// Copyright:       Copyright (C) 2022 Yuriy Trofimov
// Source Code:     https://github.com/YuriTrofimov/GameConfigModel

#include "GameParameterSource.h"

void FGameParameterSource::Startup(ULocalPlayer* InLocalPlayer, FSimpleDelegate StartupCompleteCallback)
{
	StartupCompleteCallback.ExecuteIfBound();
}
