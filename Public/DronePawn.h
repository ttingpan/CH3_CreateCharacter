#pragma once

#include "CoreMinimal.h"
#include "MainPawn.h"
#include "DronePawn.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CH3_CREATECHARACTER_API ADronePawn : public AMainPawn
{
	GENERATED_BODY()
	
protected:
	ADronePawn();

protected:
	// 키 바인딩
	virtual void SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController) override;

protected:
	// 루트 컴포넌트는 박스 컴포넌트로 설정
	UPROPERTY(VisibleAnywhere, Category = "DronePawn|Components")
	UBoxComponent* BoxComp;
	// 스태틱 메시 사용
	UPROPERTY(VisibleAnywhere, Category = "DronePawn|Components")
	UStaticMeshComponent* StaticMeshComp;
};
