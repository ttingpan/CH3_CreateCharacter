#pragma once

#include "CoreMinimal.h"
#include "MainPawn.h"
#include "PlayerPawn.generated.h"

class UCapsuleComponent;

UCLASS()
class CH3_CREATECHARACTER_API APlayerPawn : public AMainPawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

protected:
	virtual void BeginPlay() override;

	// 키 바인딩
	virtual void SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController) override;

	// 바닥 여부 설정
	virtual void UpdateIsGround() override;

	// 캐릭터 이동(WASD)
	virtual void Move(const FInputActionValue& Value) override;

	// 캐릭터 이동 업데이트
	virtual void UpdateMove(float DeltaTime) override;

	// 캐릭터 회전 업데이트
	virtual void UpdateLook(float DeltaTime) override;

	// 캐릭터 점프 시작
	UFUNCTION()
	virtual void Jump(const FInputActionValue& Value);

	// 캐릭터 달리기 시작
	UFUNCTION()
	virtual void StartSprint(const FInputActionValue& Value);

	// 캐릭터 달리기 종료
	UFUNCTION()
	virtual void StopSprint();


protected:

	// 루트 컴포넌트는 캡슐 컴포넌트로 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerPawn|Components")
	UCapsuleComponent* CapsuleComp;
	// 스켈레탈 메시 사용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerPawn|Components")
	USkeletalMeshComponent* SkeletalMeshComp;

#pragma region Move
	// 기본 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Move")
	float NormalSpeed;

	// 달리기 속도 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Move")
	float SprintSpeedMultiplier;

	// 실제 달리기 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Move")
	float SprintSpeed;

#pragma endregion

#pragma region Jump

	// 점프 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Jump")
	bool bIsJumping;

	// 점프 힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Jump")
	float JumpForce;

	// 점프 시작시 이동 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainPawn|Movement|Jump")
	FVector2D JumpStartDirection;

	// 최대 공중 이동속도 감속 비율 -> 비율이 높을 수록 더 많이 감속
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPawn|Movement|Jump", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MaxAirControlMultiplier;

#pragma endregion
};
