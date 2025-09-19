// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphNode.h"
#include "Animation/AnimationAsset.h"
#include "CableActor.h"
#include "CoreGlobals.h"
#include "Engine/Engine.h"
#include "Logging/LogVerbosity.h"
#include "Math/MathFwd.h"
#include "NodeEdge.h"
#include "Templates/Casts.h"
#include <cstddef>
#include <string>
#include "CableComponent.h"
#include "Components/StaticMeshComponent.h"

AGraphNode::AGraphNode()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);
}
// Sets default values
AGraphNode::AGraphNode(std::string name)
{
	this->name = name;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);
}

void AGraphNode::SetName(std::string name)
{
	this->name = name;
}

void AGraphNode::BeginPlay()
{
	Super::BeginPlay();
	NodeRadius = StaticMesh->Bounds.SphereRadius;
}

void AGraphNode::AddEdge(AGraphNode* Node)
{
	if (Node->name == this->name)
	{
		UE_LOG(LogTemp, Warning, TEXT("attempted to add self as edge in graph node"));
		return;
	}
	if (!edges.contains(Node->name))
	{

		edges.insert({ Node->name, Node });
		UE_LOG(LogTemp, Display,
			TEXT("Successfully added edge %s -> %s"),
			ANSI_TO_TCHAR(this->name.c_str()),
			ANSI_TO_TCHAR(Node->name.c_str()))

		if (!Node->edges.contains(this->name))
		{
			ANodeEdge* GraphEdge = GetWorld()->SpawnActor<ANodeEdge>(
				GraphEdgeBlueprint, FVector::ZeroVector, FRotator::ZeroRotator);

			GraphEdge->AttachEdge(this, Node);

			VisualEdges.insert({Node->name, GraphEdge});
		}
		else
		{
			Node->VisualEdges[this->name]->AddArrowAtStart();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Node %s already has edge to %s."),
			ANSI_TO_TCHAR(this->name.c_str()),
			ANSI_TO_TCHAR(Node->name.c_str()));
	}
}

void AGraphNode::SpawnCable(AGraphNode* FromNode, AGraphNode* ToNode)
{
}

std::string AGraphNode::GetNodeName()
{
	return this->name;
}
