// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "CanvasItem.h"
#include "Components/DecalComponent.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Engine/Font.h"
#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectMacros.h"
#include <string>
#include <unordered_map>
#include "CableActor.h"
#include "ActorSequenceComponent.h"
#include "GraphNode.generated.h"

UCLASS()

class ALGORITHM_VISUALIZER_API AGraphNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGraphNode(FString);
	AGraphNode();

	UFUNCTION()
	void UpdateDecalTexture(UCanvas* Canvas, int32 Width, int32 Height);

	UFUNCTION(BlueprintNativeEvent)
	void OnVisited(AGraphNode* NextNode);

	UFUNCTION(BlueprintNativeEvent)
	void OnTravelTo(AGraphNode* NextNode);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	UDecalComponent* NameDecal;

	UPROPERTY(EditAnywhere)
	float DecalScale = 4.0;

	UPROPERTY(EditAnywhere)
	UFont* TextFont;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCanvasRenderTarget2D> RenderTargetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ANodeEdge> GraphEdgeBlueprint;

	TMap<FString, AGraphNode*> Edges;

	UPROPERTY(BlueprintReadOnly)
	FString NodeName;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, ANodeEdge*> VisualEdges;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// In your C++ class header (.h)
protected:
	virtual void Tick(float DeltaTime) override;

	APlayerCameraManager* CameraManager;

	void SpawnCable(AGraphNode* FromNode, AGraphNode* ToNode);

	void RotateDecalToCamera();

	UCanvasRenderTarget2D* dynamicTexture;

public:
	// Called every frame
	void	SetName(FString);
	FString GetNodeName();
	void	AddEdge(AGraphNode* Node);
	float	NodeRadius;
};
