#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPawn.generated.h"

class UEnhancedInputComponent;
class AMainPlayerController;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class CH3_CREATECHARACTER_API AMainPawn : public APawn
{
	GENERATED_BODY()

public:
	AMainPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 스프링 암 컴포넌트와 카메라 컴포넌트 생성
	void CreateCamera();

	// 키 바인딩
	virtual void SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController);

	// 캐릭터 이동 시작(WASD)
	UFUNCTION()
	virtual void StartMove(const FInputActionValue& Value);

	// 캐릭터 이동 끝(WASD)
	UFUNCTION()
	virtual void StopMove(const FInputActionValue& Value);

	// 캐릭터 달리기 시작
	UFUNCTION()
	virtual void StartSprint(const FInputActionValue& Value);

	// 캐릭터 달리기 종료
	UFUNCTION()
	virtual void StopSprint(const FInputActionValue& Value);

	// 캐릭터 회전(마우스)
	UFUNCTION()
	virtual void Look(const FInputActionValue& Value);

	// 캐릭터 점프 시작
	UFUNCTION()
	virtual void StartJump(const FInputActionValue& Value);

	// 캐릭터 점프 종료
	UFUNCTION()
	virtual void StopJump(const FInputActionValue& Value);

	// 현재 속도
	UFUNCTION(BlueprintPure, Category = "MainPawn|Movement")
	float GetCurrentSpeed();

	// 이동 가능 여부
	UFUNCTION()
	virtual bool CanMoveToLocation(FVector AddValue);

protected:
#pragma region Move

	// 기본 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Move")
	float NormalSpeed;

	// 실제 이동 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	float MoveSpeed;

	// 달리기 속도 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Move")
	float SprintSpeedMultiplier;
	
	// 실제 달리기 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	float SprintSpeed;

	// 현재 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	float CurrentSpeed;

	// 이동 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	FVector MoveDirection;

	// 공중 조작 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Move")
	float AirControlMultipier;

	// 공중 이동 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	float AirMoveSpeed;

#pragma endregion

#pragma region Camera

	// 드론 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Camera")
	bool bIsDrone;

	// Roll회전 속도 - 비행체만 작동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Move")
	float RollSpeed;

	// 카메라 좌우 이동 속도(카메라 X축 민감도)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Camera")
	float CameraSpeedX;

	// 카메라 상하 이동 속도(카메라 Y축 민감도)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Camera")
	float CameraSpeedY;

	// 스프링 암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerPawn|Camera")
	USpringArmComponent* SpringArmComp;
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerPawn|Camera")
	UCameraComponent* CameraComp;

#pragma endregion

#pragma region Jump

	// 점프 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Jump")
	bool bIsJumping;

	// 점프 힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Jump")
	float JumpForce;

	// 중력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Jump")
	float Gravity;

	// 수직 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Jump")
	float VerticalVelocity;

#pragma endregion

#pragma region Collision

	// 디버그 드로우 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Debug")
	bool bShowDebugDrawing;

	// 충돌한 오브젝트 목록
	TArray<FHitResult> OutHits;
	// 콜리전 모양
	FCollisionShape MyCollision;
	// 충돌 무시할 객체를 담을 쿼리 파라미터
	FCollisionQueryParams QueryParams;

#pragma endregion
};
