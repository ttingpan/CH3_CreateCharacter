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
	Gravity = -980.0f;
	bIsGround = false;
	MoveSpeed = 0.0f;
	CurrentSpeed = 0.0f;
	Velocity = FVector::ZeroVector;
	AddVelocity = FVector::ZeroVector;

	// 카메라
	AddRotator = FRotator::ZeroRotator;
	CameraSpeedX = 1.0f;
	CameraSpeedY = 1.0f;
	MaxPitchAngle = 60.0f;

	// 자기 자신은 충돌 무시 설정
	bShowDebugDrawing = false;
	QueryParams.AddIgnoredActor(this);
}

void AMainPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 중력 적용 업데이트
	UpdateGravity(DeltaTime);
	// 회전 업데이트
	UpdateLook(DeltaTime);
	// 이동 업데이트
	UpdateMove(DeltaTime);
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

FVector AMainPawn::GetWorldVelocity(FVector InVelocity)
{
	if (InVelocity.IsNearlyZero())
	{
		InVelocity = Velocity;
	}

	return GetActorQuat().RotateVector(InVelocity);
}

void AMainPawn::UpdateGravity(float DeltaTime)
{
	Velocity.Z += (Gravity * FMath::Pow(DeltaTime, 2));

	// 추락 속도가 중력 가속도를 넘지 않도록 조정
	if (Velocity.Z < Gravity)
	{
		Velocity.Z = Gravity;
	}
}

void AMainPawn::Move(const FInputActionValue& Value)
{
	AddVelocity = Value.Get<FVector>().GetSafeNormal() * MoveSpeed;

	// 월드 좌표계로 변환
	AddVelocity = GetWorldVelocity(AddVelocity);
}

void AMainPawn::UpdateMove(float DeltaTime)
{
}

void AMainPawn::Look(const FInputActionValue& Value)
{
	FVector LookInput = Value.Get<FVector>();

	AddRotator = FRotator(LookInput.Y * CameraSpeedY, LookInput.X * CameraSpeedX, LookInput.Z);
}

void AMainPawn::UpdateLook(float DeltaTime)
{

}

float AMainPawn::GetCurrentSpeed() const
{
	return CurrentSpeed;
}

void AMainPawn::UpdateIsGround()
{
}

bool AMainPawn::IsGround()
{
	return bIsGround;
}

