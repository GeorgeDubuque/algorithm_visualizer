// Fill out your copyright notice in the Description page of Project Settings.

#include "NodeEdge.h"
#include "Engine/Engine.h"
#include "CoreGlobals.h"
#include "CableComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "GraphNode.h"

ANodeEdge::ANodeEdge()
{
	CableComponent->NumSegments = 1;
}

void ANodeEdge::AttachEdge(AGraphNode* FromNode, AGraphNode* ToNode)
{
	this->FromNode = FromNode;
	this->ToNode = ToNode;

	float	NodeDistance = FVector::Dist(FromNode->GetActorLocation(), ToNode->GetActorLocation());
	FVector CableDirection = (ToNode->GetActorLocation() - FromNode->GetActorLocation()).GetSafeNormal();

	FVector CableStart =
		FromNode->GetActorLocation() + CableDirection * FromNode->NodeRadius;

	SetActorLocation(CableStart);

	AddArrowAtEnd();
}

void ANodeEdge::AddArrowAtEnd()
{
	FVector CableDirection =
		(ToNode->GetActorLocation() - FromNode->GetActorLocation()).GetSafeNormal();
	Arrow = GetWorld()->SpawnActor<AStaticMeshActor>(
		ArrowMesh,
		ToNode->GetActorLocation() - (CableDirection * (ToNode->NodeRadius + 20)),
		CableDirection.Rotation());
	CableComponent->SetAttachEndToComponent(Arrow->GetRootComponent());
	CableComponent->EndLocation = FVector::ZeroVector;
}

void ANodeEdge::AddArrowAtStart()
{
	FVector CableDirection =
		(ToNode->GetActorLocation() - FromNode->GetActorLocation()).GetSafeNormal();
	AStaticMeshActor* Arrow = GetWorld()->SpawnActor<AStaticMeshActor>(
		ArrowMesh,
		FromNode->GetActorLocation() + (CableDirection * (ToNode->NodeRadius + 20)),
		(-CableDirection).Rotation());
	SetActorLocation(Arrow->GetActorLocation());
}

void ANodeEdge::Travel_Implementation(){

}
