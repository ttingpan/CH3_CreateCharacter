#include "MainPawn.h"
#include "MainPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

AMainPawn::AMainPawn()
	: SpringArmComp(nullptr),
	CameraComp(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// 이동
	NormalSpeed = 300.0f;
	MoveSpeed = NormalSpeed;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	CurrentSpeed = 0.0f;
	MoveDirection = FVector::ZeroVector;
	AirControlMultipier = 0.3f;
	AirMoveSpeed = 0.0f;

	// 카메라
	bIsDrone = false;
	RollSpeed = 1.0f;
	CameraSpeedX = 1.0f;
	CameraSpeedY = 1.0f;

	// 점프
	bIsJumping = false;
	JumpForce = 500.0f;
	Gravity = -980.0f;
	VerticalVelocity = 0.0f;

	// 자기 자신은 충돌 무시 설정
	bShowDebugDrawing = false;
	QueryParams.AddIgnoredActor(this);
}

void AMainPawn::BeginPlay()
{
	Super::BeginPlay();

	// 공중 조작 배율 0.0f ~ 1.0f 범위로 조정
	AirControlMultipier = FMath::Clamp(AirControlMultipier, 0.0f, 1.0f);

	// 공중 이동 속도 설정
	AirMoveSpeed = NormalSpeed * AirControlMultipier;
}

void AMainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();

#pragma region 중력 및 점프

	// 중력 적용
	VerticalVelocity += Gravity * DeltaTime;
	// Z축 입력 값
	float ZInputValue = VerticalVelocity * DeltaTime;

	// 더이상 내려 갈수 없을 경우
	if (!CanMoveToLocation(FVector(0, 0, ZInputValue)))
	{
		bIsJumping = false;
		VerticalVelocity = 0.0f;
	}
	else
	{
		Location.Z += ZInputValue;
		SetActorLocation(Location);
	}

#pragma endregion

#pragma region 이동

	FVector InputValue = MoveDirection * MoveSpeed * DeltaTime;
	CurrentSpeed = (MoveDirection * MoveSpeed).Length();

	if (CanMoveToLocation(InputValue))
	{
		AddActorLocalOffset(InputValue);
	}

#pragma endregion

}

void AMainPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced InputComponent로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
		if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
		{
			SetupInputBinding(EnhancedInput, PlayerController);
		}
	}
}

void AMainPawn::CreateCamera()
{
	// 스프링암 컴포넌트 생성 및 기본값 설정
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;

	// 카메라 컴포넌트 생성 및 기본 설정
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
}

void AMainPawn::SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController)
{
}

void AMainPawn::StartMove(const FInputActionValue& Value)
{
	// 컨트롤러가 없으면 방향 계산 못함
	if (!Controller)
	{
		return;
	}

	MoveDirection = Value.Get<FVector>().GetSafeNormal();
}

void AMainPawn::StopMove(const FInputActionValue& Value)
{
	MoveDirection = FVector::ZeroVector;
}

void AMainPawn::StartSprint(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		MoveSpeed = SprintSpeed;
	}
}

void AMainPawn::StopSprint(const FInputActionValue& Value)
{
	MoveSpeed = NormalSpeed;
}

void AMainPawn::Look(const FInputActionValue& Value)
{
	FVector LookInput = Value.Get<FVector>();

	float PitchValue = LookInput.Y * CameraSpeedY;
	// 드론이 아닐 경우
	if (!bIsDrone)
	{
		float AddedPitch = SpringArmComp->GetRelativeRotation().Pitch + PitchValue;
		// 상하 각도 제한
		AddedPitch = FMath::Clamp(AddedPitch, -60.0f, 60.0f);
		// 카메라 상하 회전
		SpringArmComp->SetRelativeRotation(FRotator(AddedPitch, 0, 0));

		PitchValue = 0.0f;
	}

	// 캐릭터 회전 및 카메라 좌우 회전
	AddActorLocalRotation(FRotator(PitchValue, LookInput.X * CameraSpeedX, LookInput.Z * RollSpeed), true);
}

void AMainPawn::StartJump(const FInputActionValue& Value)
{
	// 점프 중이 아닐 경우
	if (Value.Get<bool>() && !bIsJumping)
	{
		bIsJumping = true;
		VerticalVelocity = JumpForce;
	}
}

void AMainPawn::StopJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		VerticalVelocity = 0.0f;
	}
}

float AMainPawn::GetCurrentSpeed()
{
	return CurrentSpeed;
}

bool AMainPawn::CanMoveToLocation(FVector AddValue)
{
	return false;
}

