#include "PlayerPawn.h"
#include "MainPawn.h"
#include "MainPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"

APlayerPawn::APlayerPawn()
{
	// 캡슐 컴포넌트 생성 및 루트 컴포넌트로 등록
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCollisionProfileName(FName("CharacterMesh"));

	// 이동을 직접 코드로 제어하기 위해 물리 시뮬레이션 비활성화
	CapsuleComp->SetSimulatePhysics(false);

	// 스켈레탈 컴포넌트 생성 및 부착
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComp->SetupAttachment(RootComponent);

	// 이동을 직접 코드로 제어하기 위해 물리 시뮬레이션 비활성화
	SkeletalMeshComp->SetSimulatePhysics(false);

	CreateCamera();

	NormalSpeed = 10.0f;
	CameraSpeedX = 1.0f;
	CameraSpeedY = 1.0f;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	MyCollision = FCollisionShape::MakeCapsule(CapsuleComp->GetScaledCapsuleRadius(), CapsuleComp->GetScaledCapsuleHalfHeight());
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerPawn::SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController)
{
	// 이동 바인딩
	if (PlayerController->MoveAction)
	{
		EnhancedInput->BindAction(
			PlayerController->MoveAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerPawn::StartMove
		);

		EnhancedInput->BindAction(
			PlayerController->MoveAction,
			ETriggerEvent::Completed,
			this,
			&APlayerPawn::StopMove
		);
	}

	// 회전 바인딩
	if (PlayerController->LookAction)
	{
		EnhancedInput->BindAction(
			PlayerController->LookAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerPawn::Look
		);
	}

	// 점프 바인딩
	if (PlayerController->JumpAction)
	{
		EnhancedInput->BindAction(
			PlayerController->JumpAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerPawn::StartJump
		);

		EnhancedInput->BindAction(
			PlayerController->JumpAction,
			ETriggerEvent::Completed,
			this,
			&APlayerPawn::StopJump
		);
	}

	// 달리기 바인딩
	if (PlayerController->SprintAction)
	{
		EnhancedInput->BindAction(
			PlayerController->SprintAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerPawn::StartSprint
		);

		EnhancedInput->BindAction(
			PlayerController->SprintAction,
			ETriggerEvent::Completed,
			this,
			&APlayerPawn::StopSprint
		);
	}
}


bool APlayerPawn::CanMoveToLocation(FVector AddValue)
{
	FQuat RotationQuat = GetActorRotation().Quaternion();
	FVector TargetLocation = GetActorLocation() + RotationQuat.RotateVector(AddValue);

	if (bShowDebugDrawing)
	{
		DrawDebugCapsule(GetWorld(), TargetLocation, MyCollision.GetCapsuleHalfHeight(), MyCollision.GetSphereRadius(), FQuat::Identity, FColor::Green, false, 1.0f);
	}

	bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), TargetLocation, FQuat::Identity, ECC_WorldStatic, MyCollision, QueryParams);

	//if (bIsHit)
	//{
	//	for (auto& Hit : OutHits)
	//	{
	//		if (GEngine)
	//		{
	//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Hit Result: %s"), *Hit.GetActor()->GetName()));
	//		}
	//	}
	//}

	return !bIsHit;
}

