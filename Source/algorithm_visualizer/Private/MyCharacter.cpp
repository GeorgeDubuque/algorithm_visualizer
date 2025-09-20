// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Animation/AnimationAsset.h"
#include "Blueprint/UserWidget.h"
#include "CoreGlobals.h"
#include "Engine/EngineTypes.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/LocalPlayer.h"
#include "GraphNode.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputModifiers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "Math/MathFwd.h"
#include "RHIDefinitions.h"
#include "UMG.h"
#include "Templates/UnrealTemplate.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Add Input Mapping Context
	if (APlayerController* PlayerController =
			Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
					PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}

	UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	WidgetInstance->AddToViewport();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::OnMove(const FInputActionValue& Action)
{
	FVector2D MoveInput = Action.Get<FVector2D>();
	FVector MoveDirection = 
		GetLocalViewingPlayerController()->PlayerCameraManager->GetActorForwardVector() * MoveInput.Y + 
		GetActorRightVector() * MoveInput.X;
	AddMovementInput(MoveDirection.GetSafeNormal(), MoveSpeed);
}
void AMyCharacter::OnLook(const FInputActionValue& Action)
{
	FVector2D LookInput = Action.Get<FVector2D>();
	AddControllerPitchInput(-LookInput.Y);
	AddControllerYawInput(LookInput.X);
}

void AMyCharacter::OnClick(const FInputActionValue& Action)
{
	FHitResult Hit = LineTraceForward();
	if (Hit.IsValidBlockingHit())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Clicked: " + Hit.GetActor()->GetName()));
		LastClickedActor = Hit.GetActor();
		// check for implements GraphNode
		if (Hit.GetActor()->ActorHasTag("GraphNodeGround"))
		{
			AGraphNode* GraphNodeInstance = GetWorld()->SpawnActor<AGraphNode>(
				GraphNode, Hit.ImpactPoint, FRotator::ZeroRotator);
			GraphNodeInstance->SetName(GenerateGraphNodeName());
			GraphNodeInstance->SetActorLocation(
				GraphNodeInstance->GetActorLocation() + 
				(FVector::UpVector * GraphNodeInstance->StaticMesh->GetCollisionShape().GetSphereRadius()));

			UE_LOG(LogTemp, Display,
				TEXT("Created node with name: %s"),
				*GraphNodeInstance->GetNodeName());
			numNodes++; // increments the num nodes we have in order to generate the node name correctly
		}
	}
}

// TODO: make it so this can go on forever A, B, C,...,Z, AA, AB, AC
//  right now this will break if the num node exceeds the ascii char nums
FString AMyCharacter::GenerateGraphNodeName()
{
	char		name = static_cast<char>(A_IN_HEX + numNodes);
	FString str = FString::Chr(name);
	return str;
}

void AMyCharacter::OnReleaseClick(const FInputActionValue& Action)
{

	FHitResult Hit = LineTraceForward();
	if (Hit.IsValidBlockingHit())
	{
		// check for implements GraphNode
		if (LastClickedActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				TEXT("LastClicked Release on %s" + LastClickedActor->GetName()));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				TEXT("Hit on Release on %s" + Hit.GetActor()->GetName()));

			// check for last clicked actor
			// check if curr is a graph_node and current node is graph_node
			if (Hit.GetActor()->IsA<AGraphNode>() && LastClickedActor->IsA<AGraphNode>())
			{
				// create edge between nodes
				AGraphNode* HitGraphNode = Cast<AGraphNode>(Hit.GetActor());
				AGraphNode* LastClickedGraphNode = Cast<AGraphNode>(LastClickedActor);
				LastClickedGraphNode->AddEdge(HitGraphNode);
			}
		}
		// otherwise just create a new graph node at impact point
	}
}

FHitResult AMyCharacter::LineTraceForward()
{

	FCollisionQueryParams Query;
	FHitResult			  Hit;
	FVector				  StartPos = GetLocalViewingPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector				  EndPos =
		StartPos + GetLocalViewingPlayerController()->PlayerCameraManager->GetActorForwardVector() * 100000;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos, ECC_WorldStatic); // TODO: might need to change ECC_WorldStatic becuase what if the nodes can move
	//
	return Hit;
}
// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	InputComponent->BindAction(IA_Click, ETriggerEvent::Started, this, &AMyCharacter::OnClick);
	InputComponent->BindAction(IA_Click, ETriggerEvent::Completed, this, &AMyCharacter::OnReleaseClick);
	InputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyCharacter::OnMove);
	InputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyCharacter::OnLook);
}
