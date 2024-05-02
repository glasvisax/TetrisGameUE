#include "BaseTetrisPawn.h"
#include "BlocksShapeComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABaseTetrisPawn::ABaseTetrisPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 1400.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	BlockShapeComponent = CreateDefaultSubobject<UBlocksShapeComponent>("BlockShapeComponent");
}

void ABaseTetrisPawn::BeginPlay()
{
	Super::BeginPlay();
    check(BlockShapeComponent);
}

void ABaseTetrisPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (!PlayerInputComponent || !GetWorld()) return;
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
	PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &ABaseTetrisPawn::MoveForward);
	PlayerInputComponent->BindAction("MoveBackward", IE_Pressed, this, &ABaseTetrisPawn::MoveBackward);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ABaseTetrisPawn::MoveRight);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABaseTetrisPawn::MoveLeft);

	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, BlockShapeComponent, &UBlocksShapeComponent::MoveDown);
    PlayerInputComponent->BindAction("MakeRotation", IE_Pressed, BlockShapeComponent, &UBlocksShapeComponent::MakeRotation);
}

void ABaseTetrisPawn::MoveForward()
{
    if (!GetWorld() || !bActionInputEnabled) return;
    const auto Angle = GetCameraRotationAngle();   
    if (Angle < 25.0f && Angle > -25.0f) BlockShapeComponent->MoveBackward();
    else if (Angle > 25.0f && Angle < 60.0f) BlockShapeComponent->MoveLeft();
    else if ((Angle > 60.0f && Angle < 90.0f) || (Angle < -60.0f && Angle > -90.0f)) BlockShapeComponent->MoveForward();
    else if (Angle > -60.0f && Angle < -25.0f) BlockShapeComponent->MoveRight();
}

void ABaseTetrisPawn::MoveBackward()
{
    if (!GetWorld() || !bActionInputEnabled) return;
    const auto Angle = GetCameraRotationAngle();
    if (Angle < 25.0f && Angle > -25.0f) BlockShapeComponent->MoveForward();
    else if (Angle > 25.0f && Angle < 60.0f) BlockShapeComponent->MoveRight();
    else if ((Angle > 60.0f && Angle < 90.0f) || (Angle < -60.0f && Angle > -90.0f)) BlockShapeComponent->MoveBackward();
    else if (Angle > -60.0f && Angle < -25.0f) BlockShapeComponent->MoveLeft(); 
}

void ABaseTetrisPawn::MoveRight()
{
    if (!GetWorld() || !bActionInputEnabled) return;
    const auto Angle = GetCameraRotationAngle();
    if (Angle < 25.0f && Angle > -25.0f) BlockShapeComponent->MoveLeft();
    else if (Angle > 25.0f && Angle < 60.0f) BlockShapeComponent->MoveForward();
    else if ((Angle > 60.0f && Angle < 90.0f) || (Angle < -60.0f && Angle > -90.0f)) BlockShapeComponent->MoveRight();
    else if (Angle > -60.0f && Angle < -25.0f) BlockShapeComponent->MoveBackward();
}

void ABaseTetrisPawn::MoveLeft()
{  
    if (!GetWorld() || !bActionInputEnabled) return;
    const auto Angle = GetCameraRotationAngle();
    if (Angle < 25.0f && Angle > -25.0f) BlockShapeComponent->MoveRight();
    else if (Angle > 25.0f && Angle < 60.0f) BlockShapeComponent->MoveBackward();
    else if ((Angle > 60.0f && Angle < 90.0f) || (Angle < -60.0f && Angle > -90.0f)) BlockShapeComponent->MoveLeft();
    else if (Angle > -60.0f && Angle < -25.0f) BlockShapeComponent->MoveForward(); 
}

void ABaseTetrisPawn::MoveDown() 
{
    BlockShapeComponent->MoveDown();
}


float ABaseTetrisPawn::GetCameraRotationAngle()
{
    if (!CameraComponent || !GetWorld()) return 0.0f;

    const auto CameraForwardVector = CameraComponent->GetForwardVector();
    const auto Rotation = UKismetMathLibrary::FindLookAtRotation( FVector(CameraForwardVector.X, CameraForwardVector.Y, 0.0f).GetSafeNormal(), FVector::ForwardVector);

    return Rotation.Yaw;
}
