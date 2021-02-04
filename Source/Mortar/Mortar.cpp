#include "Mortar.h"
#include "Engine/Engine.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Mortar, "Mortar" );

void MortarScreenLog(const FString& msg)
{
	static uint64 id = 0;
	GEngine->AddOnScreenDebugMessage(id++, 5/*sec*/, FColor::Green,
		msg, false/*newer on top*/, FVector2D(1.5f, 1.5f)/*scale*/);
}