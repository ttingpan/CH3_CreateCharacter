#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CH3_CREATECHARACTER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();

	virtual void BeginPlay() override;

	// IMC_Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController|Input")
	UInputMappingContext* InputMappingContext;
	// IA_Move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController|Input")
	UInputAction* MoveAction;
	// IA_Look
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController|Input")
	UInputAction* LookAction;
	// IA_Jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController|Input")
	UInputAction* JumpAction;
	// IA_Sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController|Input")
	UInputAction* SprintAction;

	// IA_Move_Fly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController|Input")
	UInputAction* MoveFlyAction;
	// IA_Look_Fly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController|Input")
	UInputAction* LookFlyAction;
};
