// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Queue.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameMode.h"
#include "GraphNode.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectMacros.h"
#include "AlgVis_GameMode.generated.h"

struct DFS_State {
    DFS_State()
        : CurrentNode(nullptr) // make sure it's safe by default
    {}
	AGraphNode* CurrentNode;
	TSet<FString> CurrentVisitedNodes;
};

/**
 *
 */
UCLASS()
class ALGORITHM_VISUALIZER_API AAlgVis_GameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void			   BeginPlay() override;
	TMap<FString, AGraphNode*> GraphNodeMap;

public:
	TQueue<DFS_State> DFS_States;
	void DFS(AGraphNode* Node, TSet<FString>& VisitedNodes);
	void AddGraphNode(AGraphNode*);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> CurrNodeMesh;

	AActor* CurrNodeMeshInstance;

	void RunDFS(FString NodeName);
	void PlaybackDFS();
	UFUNCTION(BlueprintCallable)
	void DFS_ProcessNextState();

	DFS_State PrevState;

	UPROPERTY()
	TArray<AGraphNode*> DFSStack;

	UPROPERTY()
	float VisualizeDelay = 1.0f;

	UPROPERTY()
	TSet<FString> VisitedNodes;

	UPROPERTY()
	AGraphNode* NextNode;

	FTimerHandle DFSTimerHandle;
};
