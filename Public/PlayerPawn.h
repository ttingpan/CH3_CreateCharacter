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
	virtual void Tick(float DeltaTime) override;

	// 키 바인딩
	virtual void SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController) override;

	virtual bool CanMoveToLocation(FVector AddValue) override;

protected:

	// 루트 컴포넌트는 캡슐 컴포넌트로 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerPawn|Components")
	UCapsuleComponent* CapsuleComp;
	// 스켈레탈 메시 사용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerPawn|Components")
	USkeletalMeshComponent* SkeletalMeshComp;
};
