#include "MainGameMode.h"
#include "PlayerPawn.h"
#include "MainPlayerController.h"

AMainGameMode::AMainGameMode()
{
	// 기본 폰 클래스를 직접 만든 폰 클래스로 지정
	DefaultPawnClass = APlayerPawn::StaticClass();

	// 기본 플레이어 컨트롤러 클래스를 직접 만든 플레이어 컨트롤러로 지정
	PlayerControllerClass = AMainPlayerController::StaticClass();
}
