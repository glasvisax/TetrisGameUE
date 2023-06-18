// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseTetrisPawn.generated.h"

class UBlocksShapeComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TETRISGAME_API ABaseTetrisPawn : public APawn
{
	GENERATED_BODY()

public:

	ABaseTetrisPawn();

protected:

	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Components")
	UBlocksShapeComponent* BlockShapeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneRootComponent;

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DisableActionInput() { bActionInputEnabled = false; }
	void EnableActionInput() { bActionInputEnabled = true; }
	void MoveDown();

private:
	bool bActionInputEnabled = false;

	void MoveForward();
	void MoveBackward();
	void MoveRight();
	void MoveLeft();
	float GetCameraRotationAngle();
};

