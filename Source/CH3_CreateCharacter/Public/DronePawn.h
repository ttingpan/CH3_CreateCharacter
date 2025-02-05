#pragma once

#include "CoreMinimal.h"
#include "MainPawn.h"
#include "DronePawn.generated.h"

class UBoxComponent;

UCLASS()
class CH3_CREATECHARACTER_API ADronePawn : public AMainPawn
{
	GENERATED_BODY()
	
public:
	ADronePawn();

protected:
	virtual void BeginPlay() override;

	// 키 바인딩
	virtual void SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController) override;

	// 로컬 속도를 월드 속도로 변환하여 반환
	virtual FVector GetWorldVelocity(FVector InVelocity = FVector::ZeroVector) override;

	// 중력 적용
	virtual void UpdateGravity(float DeltaTime) override;

	// 캐릭터 이동(WASD)
	virtual void Move(const FInputActionValue& Value) override;

	// 캐릭터 이동 업데이트
	virtual void UpdateMove(float DeltaTime) override;

	// 캐릭터 회전 업데이트
	virtual void UpdateLook(float DeltaTime) override;

protected:

#pragma region Component

	// 루트 컴포넌트는 박스 컴포넌트로 설정
	UPROPERTY(VisibleAnywhere, Category = "DronePawn|Components")
	UBoxComponent* BoxComp;
	// 스태틱 메시 사용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DronePawn|Components")
	UStaticMeshComponent* StaticMeshComp;

#pragma endregion

#pragma region Move

	// 최대 기울임 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePawn|Movement|Move")
	float MaxTiltAngle;

	// 기울임 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DronePawn|Movement|Move")
	FVector TiltDirection;

	// 기울임 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePawn|Movement|Move")
	float TiltSpeed;

	// Roll 회전 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePawn|Movement|Move")
	float RollSpeed;

#pragma endregion

#pragma region Camera

	// 최대 Roll 회전 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DronePawn|Camera")
	float MaxRollAngle;
#pragma endregion
};
