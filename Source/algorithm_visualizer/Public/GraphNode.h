// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectMacros.h"
#include <string>
#include <unordered_map>
#include "CableActor.h"
#include "GraphNode.generated.h"

UCLASS()

class ALGORITHM_VISUALIZER_API AGraphNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraphNode(std::string);
	AGraphNode();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ANodeEdge> GraphEdgeBlueprint;

protected:
	// Called when the game starts or when spawned
	virtual void								 BeginPlay() override;
	std::string									 name;
	std::unordered_map<std::string, AGraphNode*> edges;
	std::unordered_map<std::string, ANodeEdge*> VisualEdges;

	void SpawnCable(AGraphNode* FromNode, AGraphNode* ToNode);

public:
	// Called every frame
	void		SetName(std::string);
	std::string GetNodeName();
	void		AddEdge(AGraphNode* Node);
	float										 NodeRadius;
};
