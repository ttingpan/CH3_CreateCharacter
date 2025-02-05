# CH3_CreateCharacter
## [7번 과제] Pawn 클래스로 3D 캐릭터 만들기
---
## 필수 과제 (기본 요구 사항)

### **필수 과제 1번 - C++ Pawn 클래스와 충돌 컴포넌트 구성**

- **컴포넌트 구성**
    - Pawn 클래스를 생성하고 충돌 컴포넌트를 루트로 설정합니다 (Capsule/Box/Sphere 중 택 1).
    - SkeletalMeshComponent, SpringArm, CameraComponent를 Pawn에 Attach하여 3인칭 시점을 만듭니다.
    - `GameMode`에서 `DefaultPawnClass`를 생성한 Pawn 클래스로 설정합니다.
- **Physics 설정**
    - Pawn 이동을 **직접** 코드로 제어하기 위해, **루트 충돌 컴포넌트 및 스켈레탈 메쉬의 물리 시뮬레이션 (Simulate Physics)을 비활성화해야 합니다.**

### **필수 과제 2번 - Enhanced Input 매핑 & 바인딩**

- **Enhanced Input 매핑**
    - WASD 키 입력, 마우스 회전을 Enhanced Input 액션으로 설정합니다.
    - `SetupPlayerInputComponent()`에서 액션들을 바인딩합니다. `MoveForward`(W/S), `MoveRight`(A/D), `Look`(마우스 이동) 등의 함수를 구현해야 합니다.
- **이동/회전 로직 구현**
    - 이동은 `AddActorLocalOffset()` 또는 `SetActorLocation()`을 통해 **직접** 구현합니다.
    - 회전은 `AddActorLocalRotation()` 등을 통해 **직접** 구현합니다.
    - 반드시 **Pawn 클래스 자체**에서 로직을 작성해야 합니다.
    - **기본 중력이나 낙하 로직은 구현하지 않습니다**. 즉, **단순 평면 이동**만 수행하면 됩니다.

---
## 도전 과제 (선택 요구 사항)

### **도전 과제 1번 - 6자유도 (6 DOF) 드론/비행체 구현 (난이도 상)**

- **축 분할 액션 구현**
    - 전/후 (Forward/Back): W(전진), S(후진)
    - 좌/우 (Left/Right): A(좌측), D(우측)
    - 상/하 (Up/Down): Space(상승), Shift(하강)
    - Yaw 회전: 마우스 X
    - Pitch 회전: 마우스 Y
    - Roll 회전 (선택사항): Q/E 키 또는 마우스 휠 등 구현
- **드론 (비행체)식 완전 자유 이동 구현**
    - 단순히 ‘똑바로’ 전진하는 것이 아닙니다. 즉시 위치/회전을 처리하는 정도에 그치면, “디스플레이 상에서 XY 평면 + Z 회전”과 크게 다를 바가 없어 보입니다 (단지 축이 늘어난 정도).
    - 즉, **“진짜 드론”** 느낌을 내고 싶다면, 단순 이동/회전 함수 호출을 넘어, **드론의 orientation (방향/기울기)을 입력으로 조정**하고, **그 orientation에 따라 전진/상승 방향이 달라지는** 구조를 만들어야 합니다.
        - 앞으로 가려면 (Pitch를 살짝 숙이거나, Roll을 기울여) 기울어진 방향으로 전진
        - 좌/우 이동도 Roll 또는 Yaw에 의해 방향이 달라질 수 있어야 합니다.
        - “입력값 → 회전 각도/기울기 → 최종 위치” 흐름을 구현하게 됩니다.
        - 즉, “이동 방향”과 “기울어진(회전된) 드론 방향”이 일치하도록 만들어야 비로소 ‘드론’ 같은 움직임이 납니다.

### **도전 과제 2번 - 중력 및 낙하 구현 (난이도 최상)**

- **Physics를 직접 활성화하지 말고**, Pawn 자체의 **Tick 로직**을 통해 낙하와 충돌을 처리합니다. (물리 시뮬레이션을 켜면 과제 의도와 어긋납니다.)
- **인공 중력 구현**
    - 엔진 내장 `CharacterMovementComponent` 없이, **Pawn 코드**에서 중력 가속도를 직접 계산해야 합니다. (낙하 속도는 적절한 중력 상수(예: -980 cm/s²) 기반으로 구현합니다.)
    - 지면에 닿지 않는 동안 Pawn은 계속 낙하해야 합니다.
    - CapsuleComponent의 바닥 부분으로 Sweep/Trace를 하거나, `AddActorOffset` 충돌 결과 등을 활용해 지면을 감지합니다.
    - 착지하면 중력 적용을 중단 (또는 Z 속도를 0)해야 합니다.
- **에어컨트롤 구현**
    - 공중에서도 어느 정도 WASD 입력을 적용할 수 있게 해야 합니다.
    - 단, 지상에서의 이동보다 **느리거나 제한적**이어야 합니다 (가령 30~50% 정도).
    - “속도/가속도” 변수를 써서 이동 로직을 좀 더 자연스럽게 (=지상/공중) 구분하면 좋습니다.

---
---
## **구현 해야할 내용**

### **충돌 컴포넌트 구성**

- 캐릭터와 드론을 구현해야 하고 공통적인 부분이 많아서 상위 부모 클래스 `AMainPanw`클래스를 만들었다.
- 공통으로 가지는 컴포넌트는 `MainPawn`클래스 에서 선언 해주고 각 하위 클래스에서 구현한다.
    - 공통 컴포넌트 : `SpringArmComponent`, `CameraComponent`
    - 캐릭터(`APlayerPawn`) : `CapusleComponent`, `SkeletalComponent`
    - 드론(`ADronePawn`) : `BoxComponent`, `StaticMeshComponent`
  
- 공통으로 충돌 컴포넌트의 물리 시뮬레이션을 비활성화 해준다.
```cpp

// 이동을 직접 코드로 제어하기 위해 물리 시뮬레이션 비활성화
CapsuleComp->SetSimulatePhysics(false);

// 이동을 직접 코드로 제어하기 위해 물리 시뮬레이션 비활성화
SkeletalMeshComp->SetSimulatePhysics(false);

```
- 각 콜리전 또한 `No Collision`으로 설정해준다.

### **Enhanced Input 매핑 & 바인딩**

- 컨트롤러와, Input Mapping Context(IMC) 등은 공통으로 사용한다.
- 캐릭터와 드론의 이동과 회전의 작동 방식이 다르기 때문에 Input Action은 각각 만들어 준다.

![image](https://github.com/user-attachments/assets/e86aa0ec-ef24-4c05-aaf4-cd494a4e9a5e)

- 캐릭터
    - `IA_Move` : 값 타입을 `Axis2D(Vector2D)`로 설정하고 `X축`은 `W`와 `S`로, `Y축`은 `A`와 `D`로 매핑한다.
    - `IA_Look` : 값 타입을 `Axis2D(Vector2D)`로 설정하고 `마우스 XY축`으로 매핑한다.
    - `IA_Jump` : 값 타입을 `Digital(bool)`로 설정하고 `스페이스바`로 매핑한다.
    - `IA_Sprint` : 값 타입을 `Digital(bool)`로 설정하고 `왼쪽 Shift`로 매핑한다.

<details>
<summary>캐릭터 이동 바인딩</summary>

```cpp

void APlayerPawn::SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController)
{
    // 이동 바인딩
    if (PlayerController->MoveAction)
    {
        EnhancedInput->BindAction(
            PlayerController->MoveAction,
            ETriggerEvent::Triggered,
            this,
            &APlayerPawn::Move
        );
    }

    // 회전 바인딩
    if (PlayerController->LookAction)
    {
        EnhancedInput->BindAction(
            PlayerController->LookAction,
            ETriggerEvent::Triggered,
            this,
            &APlayerPawn::Look
        );
    }

    // 점프 바인딩
    if (PlayerController->JumpAction)
    {
        EnhancedInput->BindAction(
            PlayerController->JumpAction,
            ETriggerEvent::Triggered,
            this,
            &APlayerPawn::Jump
        );
    }

    // 달리기 바인딩
    if (PlayerController->SprintAction)
    {
        EnhancedInput->BindAction(
            PlayerController->SprintAction,
            ETriggerEvent::Triggered,
            this,
            &APlayerPawn::StartSprint
        );

        EnhancedInput->BindAction(
            PlayerController->SprintAction,
            ETriggerEvent::Completed,
            this,
            &APlayerPawn::StopSprint
        );
    }
}

```
    
</details>

- 드론
    - `IA_Move_Fly` : 값 타입을 `Axis3D(Vector)`로 설정하고 `X축`은 `W`와 `S`로, `Y축`은 `A`와 `D`로, `Z축`은 `왼쪽 Shift`와 `스페이스바`로 매핑한다.
    - `IA_Look_Fly` : 값 타입을 `Axis3D(Vector)`로 설정하고 `마우스 XY축`으로, `Z축`은 `Q`와 `E`로 매핑한다.

<details>
<summary>드론 이동 바인딩</summary>
    
```cpp

void ADronePawn::SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController)
{
	// 이동 바인딩
	if (PlayerController->MoveFlyAction)
	{
		EnhancedInput->BindAction(
			PlayerController->MoveFlyAction,
			ETriggerEvent::Triggered,
			this,
			&ADronePawn::Move
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

```

</details>

### **이동 / 회전 로직 구현 (캐릭터 / 드론)**

- **충돌 감지**
  - 물리 시뮬레이션 및 콜리전이 비활성화 되어있기 때문에 `UWorld::SweepSingleByChannel`과 `UWorld::SweepSingleByChannel`를 활용하여 충돌을 감지했다.
  - 해당 `Sweep`의 결과에서 충돌한 면의 방향 벡터를 가져와서 내적을 통해 이동 및 회전에 활용 했다.

- 이동
  - `AMainPawn::Move` 함수로 입력을 받고 입력 값을 저장한다.
  - `AMainPawn::UpdateMove` 함수로 저장한 값을 기반으로 실제로 이동하는 로직을 구현한다.
      - 캐릭터와 드론 각 클래스에서 실제 이동 로직을 구현한다.
          > 캐릭터의 점프는 `APlayerPawn::Jump`로, 달리기는 `APlayerPawn::StartSprint` 및 `APlayerPawn::StopSprint`로 값을 저장하고 `AMainPawn::UpdateMove`에서 사용한다.
```cpp

void ADronePawn::SetupInputBinding(UEnhancedInputComponent* EnhancedInput, AMainPlayerController* PlayerController)
{
	// 이동 바인딩
	if (PlayerController->MoveFlyAction)
	{
		EnhancedInput->BindAction(
			PlayerController->MoveFlyAction,
			ETriggerEvent::Triggered,
			this,
			&ADronePawn::Move
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

```


- 회전
  - `AMainPawn::Look` 함수로 입력을 받고 입력 값을 저장한다.
  - `AMainPawn::UpdateLook` 함수로 저장한 값을 기반으로 실제로 회전하는 로직을 구현한다.
      - 캐릭터와 드론 각 클래스에서 실제 회전 로직을 구현한다.

### **중력 및 낙하**

- `AMainPawn::UpdateGravity` 함수로 지속적으로 중력을 적용하고 필요시 재정의 하여 사용한다.
- 캐릭터의 경우 `Sweep`을 사용하여 별도로 바닥을 감지한다.
  > 드론의 경우 회전 및 기울임에 따라 자동으로 `이동` 하는 형태로 작동하기 때문에 `이동`에서 충돌을 감지하여 별도로 바닥을 감지 할 필요는 없다.
- 캐릭터의 경우 공중 이동 배율을 설정하고 적용하여 처음 점프했을 방향과 다른 방향으로 이동시 점차 감소된 이동속도로 이동 하도록 했다.
















