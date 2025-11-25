// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GraphNode.h"
#include "UObject/ObjectMacros.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class ALGORITHM_VISUALIZER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	UFUNCTION(Exec)
	void RunDFS(FString NodeName);
	
};
