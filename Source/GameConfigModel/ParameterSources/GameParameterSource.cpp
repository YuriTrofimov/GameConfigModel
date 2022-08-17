#include "GameParameterSource.h"

void FGameParameterSource::Startup(ULocalPlayer* InLocalPlayer, FSimpleDelegate StartupCompleteCallback)
{
	StartupCompleteCallback.ExecuteIfBound();
}
