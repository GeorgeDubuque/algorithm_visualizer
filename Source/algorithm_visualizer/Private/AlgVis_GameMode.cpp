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

void AAlgVis_GameMode::SaveDFS_State(AGraphNode* Node, TSet<FString>& VisitedNodes)
{
	// Store State to be Playback
	DFS_State CurrState;
	CurrState.CurrentNode = Node;
	CurrState.CurrentVisitedNodes = VisitedNodes;
	DFS_States.Enqueue(CurrState);
	UE_LOG(LogTemp, Warning, TEXT("Saving State: %s"), *CurrState.CurrentNode->GetNodeName());
}

void AAlgVis_GameMode::DFS(AGraphNode* Node, TSet<FString>& VisitedNodes)
{

	DFS_AnimationStates.Add("Visit " + Node->GetNodeName());

	VisitedNodes.Add(Node->GetNodeName());
	UE_LOG(LogTemp, Display, TEXT("Visited Node: %s"), *Node->GetNodeName());

	TArray<AGraphNode*> NodeEdges;
	Node->Edges.GenerateValueArray(NodeEdges);
	for (AGraphNode* NodeEdge : NodeEdges)
	{
		if (!VisitedNodes.Contains(NodeEdge->GetNodeName()))
		{
			DFS_AnimationStates.Add("Travel " + Node->GetNodeName() + " " + NodeEdge->GetNodeName());
			DFS(NodeEdge, VisitedNodes);
			DFS_AnimationStates.Add("Travel " + NodeEdge->GetNodeName() + " " + Node->GetNodeName());
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

void AAlgVis_GameMode::PlaybackDFS()
{
	DFS_ProcessNextState();
}

void AAlgVis_GameMode::DFS_ProcessNextState()
{
	// if(IsValid(PrevState.CurrentNode)){
	//	PrevState.CurrentNode->SetActorRelativeScale3D(FVector(.9,.9,.9));
	// }

	if (Curr_DFS_Animation_Index >= DFS_AnimationStates.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("No More Animation States, resetting."));
		Curr_DFS_Animation_Index = 0;
		DFS_AnimationStates.Reset();
		return;
	}

	FString CurrState = DFS_AnimationStates[Curr_DFS_Animation_Index];
	UE_LOG(LogTemp, Warning, TEXT("Playback Curr Node: %s"), *CurrState);
	TArray<FString> ParsedAnimationCommand;
	CurrState.ParseIntoArray(ParsedAnimationCommand, TEXT(" "));
	FString AnimationType = ParsedAnimationCommand[0];

	if (AnimationType == "Visit")
	{
		FString		VisitingNodeName = ParsedAnimationCommand[1];
		AGraphNode* VisitingNode = GraphNodeMap[VisitingNodeName];
		VisitingNode->OnVisited();
	}
	else if (AnimationType == "Travel")
	{
		FString		FromNodeName = ParsedAnimationCommand[1];
		FString		ToNodeName = ParsedAnimationCommand[2];
		AGraphNode* FromNode = GraphNodeMap[FromNodeName];
		AGraphNode* ToNode = GraphNodeMap[ToNodeName];
		FromNode->OnTravelTo(ToNode);
	}
	else
	{
	}

	Curr_DFS_Animation_Index++;

	// PrevState = CurrState;

	// GetWorld()->GetTimerManager().SetTimer(
	//	DFSTimerHandle,
	//	[this, PrevState]()
	//	{
	//		ProcessDFSState(PrevState);
	//	},
	//	2.0f,
	//	false
	//);
}
