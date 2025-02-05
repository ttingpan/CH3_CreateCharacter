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
	UFUNCTION()
	void CreateCamera();

	// 키 바인딩
	virtual void SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController);

	// 로컬 속도를 월드 속도로 변환하여 반환
	UFUNCTION()
	virtual FVector GetWorldVelocity(FVector InVelocity = FVector::ZeroVector);

	// 중력 적용
	UFUNCTION()
	virtual void UpdateGravity(float DeltaTime);

	// 캐릭터 이동(WASD)
	UFUNCTION()
	virtual void Move(const FInputActionValue& Value);

	// 캐릭터 이동 업데이트
	UFUNCTION()
	virtual void UpdateMove(float DeltaTime);
	
	// 캐릭터 회전(마우스)
	UFUNCTION()
	virtual void Look(const FInputActionValue& Value);

	// 캐릭터 회전 업데이트
	UFUNCTION()
	virtual void UpdateLook(float DeltaTime);

	// 현재 속도
	UFUNCTION(BlueprintPure, Category = "MainPawn|Movement")
	float GetCurrentSpeed() const;

	// 바닥 여부 설정
	UFUNCTION()
	virtual void UpdateIsGround();

	// 바닥 여부
	UFUNCTION()
	virtual bool IsGround();

protected:
#pragma region Move
	// 중력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Move")
	float Gravity;

	// 바닥 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	bool bIsGround;

	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Move")
	float MoveSpeed;

	// 현재 이동 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	float CurrentSpeed;

	// 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	FVector Velocity;

	// 추가 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	FVector AddVelocity;

#pragma endregion

#pragma region Camera

	// 추가 각도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Camera")
	FRotator AddRotator;

	// 카메라 좌우 이동 속도(카메라 X축 민감도)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Camera", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float CameraSpeedX;

	// 카메라 상하 이동 속도(카메라 Y축 민감도)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Camera", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float CameraSpeedY;

	// 최대 Pitch 회전 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePawn|Camera")
	float MaxPitchAngle;

	// 스프링 암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerPawn|Camera")
	USpringArmComponent* SpringArmComp;
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerPawn|Camera")
	UCameraComponent* CameraComp;

#pragma endregion

#pragma region Collision

	// 디버그 드로우 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Debug")
	bool bShowDebugDrawing;

	// 충돌한 오브젝트 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Debug");
	TArray<FHitResult> OutHits;
	// 콜리전 모양
	FCollisionShape MyCollision;
	// 충돌 무시할 객체를 담을 쿼리 파라미터
	FCollisionQueryParams QueryParams;

#pragma endregion
};
