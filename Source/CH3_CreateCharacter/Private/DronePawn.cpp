#include "DronePawn.h"
#include "MainPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

ADronePawn::ADronePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// 박스 컴포넌트 생성 및 루트 컴포넌트로 등록
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComp);
	BoxComp->SetCollisionProfileName(FName("CharacterMesh"));

	// 이동을 직접 코드로 제어하기 위해 물리 시뮬레이션 비활성화
	BoxComp->SetSimulatePhysics(false);

	// 스태틱 메시 컴포넌트 생성 및 부착
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(RootComponent);

	// 이동을 직접 코드로 제어하기 위해 물리 시뮬레이션 비활성화
	StaticMeshComp->SetSimulatePhysics(false);

	CreateCamera();

	SpringArmComp->bInheritRoll = false;

	// 이동
	Gravity = -980.0f;
	MoveSpeed = 300.0f;
	MaxTiltAngle = 10.0f;
	TiltDirection = FVector::ZeroVector;
	TiltSpeed = 5.0f;
	RollSpeed = 2.0f;

	// 회전
	MaxPitchAngle = 30.0f;
	MaxRollAngle = 10.0f;
}

void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

	MyCollision = FCollisionShape::MakeBox(BoxComp->GetScaledBoxExtent());
}

void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced InputComponent로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
		if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
		{
			// 이동 바인딩
			if (PlayerController->MoveFlyAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveFlyAction,
					ETriggerEvent::Triggered,
					this,
					&ADronePawn::Move
				);
			}

			// 회전 바인딩
			if (PlayerController->LookFlyAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookFlyAction,
					ETriggerEvent::Triggered,
					this,
					&ADronePawn::Look
				);
			}
		}
	}
}

FVector ADronePawn::GetWorldVelocity(FVector InVelocity)
{
	if (InVelocity.IsNearlyZero())
	{
		InVelocity = Velocity;
	}

	return StaticMeshComp->GetComponentQuat().RotateVector(InVelocity);
}

void ADronePawn::UpdateGravity(float DeltaTime)
{
	Velocity.Z = Gravity * DeltaTime;
}

void ADronePawn::Move(const FInputActionValue& Value)
{
	AddVelocity = Value.Get<FVector>().GetSafeNormal() * MoveSpeed;

	// 월드 좌표계로 변환
	AddVelocity = GetWorldVelocity(AddVelocity);

	TiltDirection = Value.Get<FVector>().GetSafeNormal();
}

void ADronePawn::UpdateMove(float DeltaTime)
{
	FVector UpVector = StaticMeshComp->GetUpVector() * DeltaTime;

	Velocity += UpVector * -Gravity;

	Velocity += UpVector * AddVelocity.Z;

	AddVelocity.Z = 0.0f;

	Velocity += AddVelocity * DeltaTime;

	FQuat RotationQuat = StaticMeshComp->GetComponentQuat();
	FVector TargetLocation = GetActorLocation() + Velocity;
	if (bShowDebugDrawing)
	{
		DrawDebugBox(GetWorld(), TargetLocation, MyCollision.GetExtent(), RotationQuat, FColor::Green, false, 0.5f);
	}

	bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), TargetLocation, RotationQuat, ECC_WorldStatic, MyCollision, QueryParams);

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

	// 이동
	AddActorWorldOffset(Velocity);

	FRotator TargetRotation = StaticMeshComp->GetRelativeRotation();

	// 이동시 기울어짐 적용
	TargetRotation.Pitch = FMath::Clamp(-TiltDirection.X * MaxTiltAngle, -MaxTiltAngle, MaxTiltAngle);
	TargetRotation.Roll = FMath::Clamp(TiltDirection.Y * MaxTiltAngle, -MaxTiltAngle, MaxTiltAngle);

	// 기울임 보간
	FRotator NewRotation = FMath::RInterpTo(StaticMeshComp->GetRelativeRotation(), TargetRotation, DeltaTime, TiltSpeed);
	StaticMeshComp->SetRelativeRotation(NewRotation);

	TiltDirection = FVector::ZeroVector;
	AddVelocity = FVector::ZeroVector;
	Velocity = FVector::ZeroVector;
}

void ADronePawn::UpdateLook(float DeltaTime)
{
	if (!AddRotator.IsNearlyZero())
	{
		AddRotator = GetActorRotation() + (AddRotator * 20.0f * DeltaTime);

		if (bShowDebugDrawing)
		{
			DrawDebugBox(GetWorld(), GetActorLocation(), MyCollision.GetExtent(), FQuat(AddRotator), FColor::Green, false, 1.0f);
		}

		bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), GetActorLocation(), FQuat(AddRotator), ECC_WorldStatic, MyCollision, QueryParams);

		if (!bIsHit)
		{
			// Pitch, Roll 회전 각도 제한
			float NewPitch = FMath::Clamp(AddRotator.Pitch, -MaxPitchAngle, MaxPitchAngle);
			float NewRoll = FMath::Clamp(AddRotator.Roll, -MaxRollAngle, MaxRollAngle);

			SetActorRotation(FRotator(NewPitch, AddRotator.Yaw, NewRoll));
		}
	}

	// 입력 없을시 회전 초기화 -> 회전 정지
	AddRotator = FRotator::ZeroRotator;
}
