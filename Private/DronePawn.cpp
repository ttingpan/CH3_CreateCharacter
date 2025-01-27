#include "DronePawn.h"
#include "MainPawn.h"
#include "MainPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

ADronePawn::ADronePawn()
{
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

	NormalSpeed = 10.0f;
	RollSpeed = 1.0f;
	CameraSpeedX = 0.5f;
	CameraSpeedY = 0.5f;
	bIsDrone = true;
}

void ADronePawn::SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController)
{
	// 이동 바인딩
	if (PlayerController->MoveFlyAction)
	{
		EnhancedInput->BindAction(
			PlayerController->MoveFlyAction,
			ETriggerEvent::Triggered,
			this,
			&ADronePawn::StartMove
		);

		EnhancedInput->BindAction(
			PlayerController->MoveFlyAction,
			ETriggerEvent::Completed,
			this,
			&ADronePawn::StopMove
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
