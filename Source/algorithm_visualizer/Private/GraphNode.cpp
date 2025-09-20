// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphNode.h"
#include "CanvasItem.h"
#include "Containers/UnrealString.h"
#include "CoreGlobals.h"
#include "Engine/Texture.h"
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
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);
	NameDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	NameDecal->SetupAttachment(RootComponent);
}
// Sets default values
AGraphNode::AGraphNode(FString name)
{
	this->name = name;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);
	NameDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	NameDecal->SetupAttachment(RootComponent);
}

void AGraphNode::UpdateDecalTexture(UCanvas* Canvas, int32 Width, int32 Height)
{
	FCanvasTextItem TextItem = FCanvasTextItem(
		FVector2D(Width / 2.0, Height / 2.0),
		FText::FromString(*this->name),
		TextFont,
		FLinearColor::White);

	TextItem.Scale = FVector2D(DecalScale, DecalScale);
	TextItem.bCentreX = true;
	TextItem.bCentreY = true;

	Canvas->DrawItem(TextItem);

	UMaterialInstanceDynamic* NewMat = NameDecal->CreateDynamicMaterialInstance();
	NewMat->SetTextureParameterValue("TP2d", dynamicTexture);
	NameDecal->SetDecalMaterial(NewMat);

}

void AGraphNode::SetName(FString name)
{
	this->name = name;

	dynamicTexture->UpdateResource();
}

void AGraphNode::BeginPlay()
{
	Super::BeginPlay();
	NodeRadius = StaticMesh->Bounds.SphereRadius;
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
}

void AGraphNode::AddEdge(AGraphNode* Node)
{
	if (Node->name == this->name)
	{
		UE_LOG(LogTemp, Warning, TEXT("attempted to add self as edge in graph node"));
		return;
	}
	if (!edges.Contains(Node->name))
	{

		edges.Add({ Node->name, Node });

		if (!Node->edges.Contains(this->name))
		{
			ANodeEdge* GraphEdge = GetWorld()->SpawnActor<ANodeEdge>(
				GraphEdgeBlueprint, FVector::ZeroVector, FRotator::ZeroRotator);

			GraphEdge->AttachEdge(this, Node);

			VisualEdges.Add({ Node->name, GraphEdge });
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
			*this->name,
			*this->name);
	}
}

void AGraphNode::SpawnCable(AGraphNode* FromNode, AGraphNode* ToNode)
{
}

FString AGraphNode::GetNodeName()
{
	return this->name;
}
