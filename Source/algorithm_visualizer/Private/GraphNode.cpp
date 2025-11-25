// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphNode.h"
#include "CanvasItem.h"
#include "Containers/UnrealString.h"
#include "CoreGlobals.h"
#include "Engine/Texture.h"
#include "GameFramework/PlayerController.h"
#include "Internationalization/Text.h"
#include "Logging/LogVerbosity.h"
#include "Math/MathFwd.h"
#include "NodeEdge.h"
#include "Templates/Casts.h"
#include "Engine.h"
#include <string>
#include "Components/StaticMeshComponent.h"

AGraphNode::AGraphNode()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a root component FIRST
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Skeletal Mesh
	StaticMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	// Decal
	NameDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	NameDecal->SetupAttachment(StaticMesh);
}
// Sets default values
AGraphNode::AGraphNode(FString name)
{
	PrimaryActorTick.bCanEverTick = true;
	this->NodeName = name;
	PrimaryActorTick.bCanEverTick = true;

	// Create a root component FIRST
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Skeletal Mesh
	StaticMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	// Decal
	NameDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	NameDecal->SetupAttachment(StaticMesh);
}

void AGraphNode::BeginPlay()
{
	Super::BeginPlay();
	NodeRadius = StaticMesh->GetCollisionShape().GetSphereRadius();
	// Now GetWorld() is safe to call
	if (GetWorld() && RenderTargetClass)
	{
		dynamicTexture = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(GetWorld(), RenderTargetClass);
		if (dynamicTexture)
		{
			dynamicTexture->OnCanvasRenderTargetUpdate.AddDynamic(this, &AGraphNode::UpdateDecalTexture);
			dynamicTexture->UpdateResource();
		}
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		CameraManager = PlayerController->PlayerCameraManager;
	}
}

void AGraphNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateDecalToCamera();
}

void AGraphNode::RotateDecalToCamera()
{
	if (CameraManager)
	{
		FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
		FVector DecalLocation = NameDecal->GetComponentLocation();
		FVector DirectionToCamera = (CameraLocation - DecalLocation).GetSafeNormal() * -1.0f;

		FRotator LookAtRotation = DirectionToCamera.Rotation();

		// Adjust for decal orientation
		LookAtRotation.Roll += 90.0f;

		NameDecal->SetWorldRotation(LookAtRotation);
		NameDecal->SetWorldLocation(GetActorLocation() + DirectionToCamera.Normalize() * 2);
	}
}

void AGraphNode::UpdateDecalTexture(UCanvas* Canvas, int32 Width, int32 Height)
{
	FCanvasTextItem TextItem = FCanvasTextItem(
		FVector2D(Width / 2.0, Height / 2.0),
		FText::FromString(*this->NodeName),
		TextFont,
		FLinearColor::White);

	TextItem.Scale = FVector2D(DecalScale, 4.0);
	TextItem.bCentreX = true;
	TextItem.bCentreY = true;

	Canvas->DrawItem(TextItem);

	UMaterialInstanceDynamic* NewMat = NameDecal->CreateDynamicMaterialInstance();
	NewMat->SetTextureParameterValue("TP2d", dynamicTexture);
	NameDecal->SetDecalMaterial(NewMat);
}

void AGraphNode::SetName(FString name)
{
	this->NodeName = name;

	dynamicTexture->UpdateResource();
}

void AGraphNode::AddEdge(AGraphNode* Node)
{
	if (Node->NodeName == this->NodeName)
	{
		UE_LOG(LogTemp, Warning, TEXT("attempted to add self as edge in graph node"));
		return;
	}
	if (!Edges.Contains(Node->NodeName))
	{

		Edges.Add({ Node->NodeName, Node });

		if (!Node->Edges.Contains(this->NodeName))
		{
			ANodeEdge* GraphEdge = GetWorld()->SpawnActor<ANodeEdge>(
				GraphEdgeBlueprint, FVector::ZeroVector, FRotator::ZeroRotator);

			GraphEdge->AttachEdge(this, Node);

			VisualEdges.Add({ Node->NodeName, GraphEdge });
		}
		else
		{
			Node->VisualEdges[this->NodeName]->AddArrowAtStart();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Node %s already has edge to %s."),
			*this->NodeName,
			*this->NodeName);
	}
}

void AGraphNode::SpawnCable(AGraphNode* FromNode, AGraphNode* ToNode)
{
}

void AGraphNode::OnVisited_Implementation()
{
}

void AGraphNode::OnTravelTo_Implementation(AGraphNode* NextNode)
{
}

FString AGraphNode::GetNodeName()
{
	return this->NodeName;
}
