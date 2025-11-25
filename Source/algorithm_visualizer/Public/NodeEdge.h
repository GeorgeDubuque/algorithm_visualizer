// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "CableActor.h"
#include "Engine/StaticMeshActor.h"
#include "GraphNode.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectMacros.h"
#include "NodeEdge.generated.h"

/**
 *
 */
UCLASS()
class ALGORITHM_VISUALIZER_API ANodeEdge : public ACableActor
{
	GENERATED_BODY()
	
public:
	ANodeEdge();
	void AttachEdge(AGraphNode* FromNode, AGraphNode* ToNode);

	void AddArrowAtStart();
	void AddArrowAtEnd();

	AGraphNode* FromNode;
	AGraphNode* ToNode;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStaticMeshActor> ArrowMesh;

	UPROPERTY(BlueprintReadonly)
	AStaticMeshActor* Arrow;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Travel();
};
