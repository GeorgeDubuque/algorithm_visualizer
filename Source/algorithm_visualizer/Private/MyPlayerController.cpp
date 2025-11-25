// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "AlgVis_GameMode.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"

void AMyPlayerController::RunDFS(FString NodeName){
	AAlgVis_GameMode* GameMode = Cast<AAlgVis_GameMode>(UGameplayStatics::GetGameMode(this)); 
	GameMode->RunDFS(NodeName);
	GameMode->PlaybackDFS();
}

