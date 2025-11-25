// Fill out your copyright notice in the Description page of Project Settings.

#include "AlgVis_GameMode.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "CoreGlobals.h"
#include "GraphNode.h"
#include "Engine/World.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"
#include "Math/MathFwd.h"
#include "MyPlayerController.h"
#include "TimerManager.h" // Add this line
#include "ActorSequenceComponent.h"
#include <cstddef>

void AAlgVis_GameMode::AddGraphNode(AGraphNode* Node)
{
	GraphNodeMap.Add({ Node->NodeName, Node });
}

// In your .cpp file
void AAlgVis_GameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("GameMode BeginPlay called!"));

	CurrNodeMeshInstance = GetWorld()->SpawnActor<AActor>(CurrNodeMesh);
}

void AAlgVis_GameMode::DFS(AGraphNode* Node, TSet<FString>& VisitedNodes)
{

	// Store State to be Playback
	DFS_State CurrState;
	CurrState.CurrentNode = Node;
	CurrState.CurrentVisitedNodes = VisitedNodes;
	DFS_States.Enqueue(CurrState);

	VisitedNodes.Add(Node->GetNodeName());
	UE_LOG(LogTemp, Display, TEXT("Visited Node: %s"), *Node->GetNodeName());

	TArray<AGraphNode*> NodeEdges;
	Node->Edges.GenerateValueArray(NodeEdges);
	for (AGraphNode* NodeEdge : NodeEdges)
	{
		if (!VisitedNodes.Contains(NodeEdge->GetNodeName()))
		{
			DFS(NodeEdge, VisitedNodes);
		}
	}
}

void AAlgVis_GameMode::RunDFS(FString StartingNodeName)
{
	if (!GraphNodeMap.Contains(StartingNodeName))
	{
		UE_LOG(LogTemp, Error, TEXT("Starting node name is invalid not running DFS."));
	}

	TSet<FString> VisitedNodes;
	AGraphNode*	  StartingNode = *GraphNodeMap.Find(StartingNodeName);
	DFS(StartingNode, VisitedNodes);
}

void AAlgVis_GameMode::PlaybackDFS(){
	DFS_ProcessNextState();
}

void AAlgVis_GameMode::DFS_ProcessNextState(){
	//if(IsValid(PrevState.CurrentNode)){
	//	PrevState.CurrentNode->SetActorRelativeScale3D(FVector(.9,.9,.9));
	//}

	if(DFS_States.IsEmpty()) return;

	DFS_State CurrState;
	DFS_States.Dequeue(CurrState);
	UE_LOG(LogTemp, Warning, TEXT("Playback Curr Node: %s"), *CurrState.CurrentNode->NodeName);
	DFS_State NextState;
	DFS_States.Peek(NextState);
	CurrState.CurrentNode->OnVisited(NextState.CurrentNode);

	//CurrState.CurrentNode->OnTravelTo(NextState.CurrentNode);
	PrevState = CurrState;


	//PrevState = CurrState;

	//GetWorld()->GetTimerManager().SetTimer(
	//	DFSTimerHandle,
	//	[this, PrevState]()
	//	{
	//		ProcessDFSState(PrevState);
	//	},
	//	2.0f,
	//	false
	//);
}
