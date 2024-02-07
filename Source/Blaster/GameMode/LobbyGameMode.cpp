// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

/**
 * @brief 房主在大厅地图等待其他玩家加入游戏，当加入游戏的玩家数量超过指定的个数，将地图切换到对战地图
 * @param NewPlayer 
 */
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		bUseSeamlessTravel = true;
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
	}
}
