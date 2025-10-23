// Copyright Peasant Pals 2025


#include "Core/GameStateEventManagerBase.h"

void AGameStateEventManagerBase::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	MatchEnded.Broadcast();
}

void AGameStateEventManagerBase::SetHasPlayerWon(bool InHasPlayerWon)
{
	_mHasPlayerWon = InHasPlayerWon;
}

bool AGameStateEventManagerBase::GetHasPlayerWon()
{
	return _mHasPlayerWon;
}
