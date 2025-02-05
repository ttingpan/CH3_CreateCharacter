#include "PlayerPawn.h"
#include "MainPawn.h"
#include "MainPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

	// 점프
	bIsJumping = false;
	JumpForce = 300.0f;
	JumpStartDirection = FVector2D::ZeroVector;
	MaxAirControlMultiplier = 0.5f;

	// 달리기
	NormalSpeed = 300.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = 0.0f;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	MoveSpeed = NormalSpeed;

	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	MyCollision = FCollisionShape::MakeCapsule(CapsuleComp->GetScaledCapsuleRadius(), CapsuleComp->GetScaledCapsuleHalfHeight());
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
			&APlayerPawn::Move
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
			&APlayerPawn::Jump
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

void APlayerPawn::UpdateIsGround()
{
	FVector VerticalVelocity = FVector(0.0f, 0.0f, Velocity.Z);
	FVector TargetLocation = GetActorLocation() + VerticalVelocity;

	if (bShowDebugDrawing)
	{
		//DrawDebugCapsule(GetWorld(), TargetLocation, MyCollision.GetCapsuleHalfHeight(), MyCollision.GetSphereRadius(), FQuat::Identity, FColor::Green, false, 0.5f);
	}

	FHitResult Hit;

	bool bIsHit = GetWorld()->SweepSingleByChannel(Hit, GetActorLocation(), TargetLocation, FQuat::Identity, ECC_WorldStatic, MyCollision, QueryParams);

	// 중력 방향과 바닥 면과의 내적
	float Scalar = FVector::DotProduct(FVector(0, 0, -1.0f), Hit.ImpactNormal);

	bIsGround = bIsHit && FMath::IsNearlyEqual(Scalar, -1.0f, 0.5f);

	if (bIsGround)
	{
		bIsJumping = false;
	}
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	Super::Move(Value);

	// 바닥이 아닐 경우
	if (!IsGround())
	{
		FVector2D MoveDirection = FVector2D(GetWorldVelocity(Value.Get<FVector>().GetSafeNormal()));

		// 두 방향 내적해서 얼마나 차이나는지 확인
		float Scalar = FVector2D::DotProduct(JumpStartDirection, MoveDirection);

		Scalar = FMath::Clamp(Scalar, MaxAirControlMultiplier, 1.0f);

		AddVelocity *= Scalar;
	}
	else
	{
		JumpStartDirection = FVector2D::ZeroVector;
	}
}

void APlayerPawn::UpdateMove(float DeltaTime)
{
	// 바닥 여부 확인
	UpdateIsGround();

	// 바닥이면 현재 Z축 속도 제거
	if (IsGround())
	{
		Velocity.Z = 0.0f;
	}

	Velocity += AddVelocity * DeltaTime;

	FVector TargetLocation = GetActorLocation() + Velocity;

	if (bShowDebugDrawing)
	{
		DrawDebugCapsule(GetWorld(), TargetLocation, MyCollision.GetCapsuleHalfHeight(), MyCollision.GetSphereRadius(), FQuat::Identity, FColor::Green, false, 0.5f);
	}

	bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), TargetLocation, FQuat::Identity, ECC_WorldStatic, MyCollision, QueryParams);

	if (bIsHit)
	{
		// 충돌 면과 내적 계산
		for (const auto& Hit : OutHits)
		{
			auto HitDirection = Hit.ImpactNormal;

			// 내적 계산해서 Velocity에 반영
			float Scalar = FVector::DotProduct(Velocity, HitDirection);

			Velocity -= Scalar * HitDirection;
		}
	}

	if (!Velocity.IsNearlyZero())
	{
		CurrentSpeed = MoveSpeed;
	}
	else
	{
		CurrentSpeed = 0.0f;
	}

	// 이동
	AddActorWorldOffset(Velocity);

	AddVelocity = FVector::ZeroVector;
	Velocity = FVector(0, 0, Velocity.Z);
}

void APlayerPawn::UpdateLook(float DeltaTime)
{
	if (!AddRotator.IsNearlyZero())
	{
		AddRotator *= (DeltaTime * 20.0f);

		float AddedPitch = SpringArmComp->GetRelativeRotation().Pitch + AddRotator.Pitch;

		// Pitch 회전 각도 제한
		AddedPitch = FMath::Clamp(AddedPitch, -MaxPitchAngle, MaxPitchAngle);

		// 카메라 상하 회전
		SpringArmComp->SetRelativeRotation(FRotator(AddedPitch, 0.0f, 0.0f));

		AddRotator.Pitch = 0.0f;

		// 캐릭터 회전 및 카메라 좌우 회전
		AddActorLocalRotation(AddRotator);
	}

	// 입력 없을시 회전 초기화 -> 회전 정지
	AddRotator = FRotator::ZeroRotator;
}

void APlayerPawn::Jump(const FInputActionValue& Value)
{
	// 점프 중이 아닐 경우
	if (Value.Get<bool>() && !bIsJumping)
	{
		bIsJumping = true;
		AddVelocity.Z += JumpForce;

		// 점프 시작시 방향 -> 월드 축 기준
		JumpStartDirection = FVector2D(GetWorldVelocity(AddVelocity)).GetSafeNormal();
	}
}

void APlayerPawn::StartSprint(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		// 달리기 속도로 설정
		MoveSpeed = SprintSpeed;
	}
}

void APlayerPawn::StopSprint()
{
	// 기본 이동속도로 설정
	MoveSpeed = NormalSpeed;
}

