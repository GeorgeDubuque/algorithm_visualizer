// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/HitResult.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "GraphNode.h"
#include "InputActionValue.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectMacros.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "MyCharacter.generated.h"

UCLASS()
class ALGORITHM_VISUALIZER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(EditAnywhere)
	UInputMappingContext* IMC_Default;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Click;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGraphNode> GraphNode;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(VisibleAnywhere)
	float MoveSpeed = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void	OnClick(const FInputActionValue& Action);
	void	OnReleaseClick(const FInputActionValue& Action);
	void	OnMove(const FInputActionValue& Action);
	void	OnLook(const FInputActionValue& Action);
	FHitResult	LineTraceForward();
	AActor* LastClickedActor;
	int		A_IN_HEX = 65;
	int		numNodes = 0;

private:
	std::string GenerateGraphNodeName();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
