# CH3_CreateCharacter
## [7번 과제] Pawn 클래스로 3D 캐릭터 만들기
---
## 필수 과제 (기본 요구 사항)

### **필수 과제 1번 - C++ Pawn 클래스와 충돌 컴포넌트 구성**

- **컴포넌트 구성**
     
	<details>
	<summary>세부 사항</summary>
	
	- Pawn 클래스를 생성하고 충돌 컴포넌트를 루트로 설정합니다 (Capsule/Box/Sphere 중 택 1).
	- SkeletalMeshComponent, SpringArm, CameraComponent를 Pawn에 Attach하여 3인칭 시점을 만듭니다.
	- `GameMode`에서 `DefaultPawnClass`를 생성한 Pawn 클래스로 설정합니다.
	
	</details>

- **Physics 설정**
  
	<details>
	<summary>세부 사항</summary>
	
	- Pawn 이동을 **직접** 코드로 제어하기 위해, **루트 충돌 컴포넌트 및 스켈레탈 메쉬의 물리 시뮬레이션 (Simulate Physics)을 비활성화해야 합니다.**
	 
	</details>

### **필수 과제 2번 - Enhanced Input 매핑 & 바인딩**

- **Enhanced Input 매핑**
  
	<details>
	<summary>세부 사항</summary>
	
	- WASD 키 입력, 마우스 회전을 Enhanced Input 액션으로 설정합니다.
	- `SetupPlayerInputComponent()`에서 액션들을 바인딩합니다. `MoveForward`(W/S), `MoveRight`(A/D), `Look`(마우스 이동) 등의 함수를 구현해야 합니다.
	 
	</details>

- **이동/회전 로직 구현**

	<details>
	<summary>세부 사항</summary>
	
	- 이동은 `AddActorLocalOffset()` 또는 `SetActorLocation()`을 통해 **직접** 구현합니다.
	- 회전은 `AddActorLocalRotation()` 등을 통해 **직접** 구현합니다.
	- 반드시 **Pawn 클래스 자체**에서 로직을 작성해야 합니다.
	- **기본 중력이나 낙하 로직은 구현하지 않습니다**. 즉, **단순 평면 이동**만 수행하면 됩니다.
	 
	</details>

---
## 도전 과제 (선택 요구 사항)

### **도전 과제 1번 - 6자유도 (6 DOF) 드론/비행체 구현 (난이도 상)**

- **축 분할 액션 구현**

	<details>
	<summary>세부 사항</summary>
	
	- **이동**
	    - 전/후 (W/S) - X축 이동
	    - 좌/우 (A/D) - Y축 이동
	    - 상/하 (Space/Shift) - Z축 이동
	- **회전**
	    - Yaw - 좌우 회전, 마우스 이동 (Z축 회전)
	    - Pitch - 상화 회전, 마우스 이동 (Y축 회전)
	    - Roll - 기울기 회전, 마우스 휠 (X축 회전)
	 
	</details>

 - **Orientation 기반 이동 구현**

	<details>
	<summary>세부 사항</summary>
	
	- 단순 ‘평면 이동’이 아니라, *Pitch*나 *Roll* 각도를 기울여 전진·선회하는 식의 비행체 움직임 구현합니다.
    	- AddActorOffset/Rotation 호출 전에 **현재 회전 상태** (방향·기울기)에 따라 입력 벡터를 변환하여 적용합니다.
	 
	</details>

### **도전 과제 2번 - 중력 및 낙하 구현 (난이도 최상)**

- **인공 중력 구현**

	<details>
	<summary>세부 사항</summary>
	
	- **Physics 시뮬레이션 대신 Tick 로직**을 통해 인공 중력을 구현합니다.
	- 엔진 내장 `CharacterMovementComponent` 없이, **Pawn 코드**에서 매 프레임 중력 가속도를 직접 계산해야 합니다. (낙하 속도는 적절한 중력 상수 (예: -980 cm/s²) 기반으로 구현합니다.)
	- 바닥 충돌 판정을 해야합니다. (Sweep/Trace를 하거나, `AddActorOffset` 충돌 결과 등을 활용해 지면을 감지합니다.)
	- 착지 시 낙하 중단 또는 Z 속도를 0으로 초기화해야 합니다.
	 
	</details> 

- **에어컨트롤 구현 (공중 WASD 제어)**

	<details>
	<summary>세부 사항</summary>
	
	- 공중에서는 지상에서의 이동보다 **느리거나 제한적**이어야 합니다 (가령 30~50% 정도).
	- 낙하와 착지가 시각적으로 자연스럽게 처리될 수 있도록, 속도/가속도 변수를 써서 이동 로직을 좀 더 자연스럽게 (=지상/공중) 구분하면 좋습니다.
	 
	</details>

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

---

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

---

### **이동 / 회전 로직 구현 (캐릭터 / 드론)**

- **충돌 감지**
  - 물리 시뮬레이션 및 콜리전이 비활성화 되어있기 때문에 `UWorld::SweepSingleByChannel`과 `UWorld::SweepSingleByChannel`를 활용하여 충돌을 감지했다.
  - 해당 `Sweep`의 결과에서 충돌한 면의 방향 벡터를 가져와서 내적을 통해 이동 및 회전에 활용 했다.

- 이동
  - `AMainPawn::Move` 함수로 입력을 받고 입력 값을 저장한다.
  - `AMainPawn::UpdateMove` 함수로 저장한 값을 기반으로 실제로 이동하는 로직을 구현한다.
      - 캐릭터와 드론 각 클래스에서 실제 이동 로직을 구현한다.
          > 캐릭터의 점프는 `APlayerPawn::Jump`로, 달리기는 `APlayerPawn::StartSprint` 및 `APlayerPawn::StopSprint`로 값을 저장하고 `AMainPawn::UpdateMove`에서 사용한다.

- **캐릭터 이동 코드 예시**

<details>
<summary>AMainPawn::Move</summary>

```cpp

void AMainPawn::Move(const FInputActionValue& Value)
{
	AddVelocity = Value.Get<FVector>().GetSafeNormal() * MoveSpeed;

	// 월드 좌표계로 변환
	AddVelocity = GetWorldVelocity(AddVelocity);
}

```
 
</details>

<details>
<summary>APlayerPawn::Move</summary>

```cpp

void APlayerPawn::Move(const FInputActionValue& Value)
{
	Super::Move(Value);

	// 바닥이 아닐 경우
	if (!IsGround())
	{
		FVector2D MoveDirection = FVector2D(GetWorldVelocity(Value.Get<FVector>().GetSafeNormal()));

		// 두 방향 내적해서 얼마나 차이나는지 확인
		float Scalar = FVector2D::DotProduct(JumpStartDirection, MoveDirection);

		Scalar = FMath::Clamp(Scalar, MaxAirControlMultiplier, 1.0f);

		AddVelocity *= Scalar;
	}
	else
	{
		JumpStartDirection = FVector2D::ZeroVector;
	}
}

```
 
</details>

<details>
<summary>APlayerPawn::UpdateMove</summary>

```cpp

void APlayerPawn::UpdateMove(float DeltaTime)
{
	// 바닥 여부 확인
	UpdateIsGround();

	// 바닥이면 현재 Z축 속도 제거
	if (IsGround())
	{
		Velocity.Z = 0.0f;
	}

	Velocity += AddVelocity * DeltaTime;

	FVector TargetLocation = GetActorLocation() + Velocity;

	if (bShowDebugDrawing)
	{
		DrawDebugCapsule(GetWorld(), TargetLocation, MyCollision.GetCapsuleHalfHeight(), MyCollision.GetSphereRadius(), FQuat::Identity, FColor::Green, false, 0.5f);
	}

	bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), TargetLocation, FQuat::Identity, ECC_WorldStatic, MyCollision, QueryParams);

	if (bIsHit)
	{
		// 충돌 면과 내적 계산
		for (const auto& Hit : OutHits)
		{
			auto HitDirection = Hit.ImpactNormal;

			// 내적 계산해서 Velocity에 반영
			float Scalar = FVector::DotProduct(Velocity, HitDirection);

			Velocity -= Scalar * HitDirection;
		}
	}

	if (!Velocity.IsNearlyZero())
	{
		CurrentSpeed = MoveSpeed;
	}
	else
	{
		CurrentSpeed = 0.0f;
	}

	// 이동
	AddActorWorldOffset(Velocity);

	AddVelocity = FVector::ZeroVector;
	Velocity = FVector(0, 0, Velocity.Z);
}

```
 
</details>

---

- 회전
  - `AMainPawn::Look` 함수로 입력을 받고 입력 값을 저장한다.
  - `AMainPawn::UpdateLook` 함수로 저장한 값을 기반으로 실제로 회전하는 로직을 구현한다.
      - 캐릭터와 드론 각 클래스에서 실제 회전 로직을 구현한다.

- **캐릭터 회전 코드 예시**

<details>
<summary>AMainPawn::Look</summary>

```cpp

void AMainPawn::Look(const FInputActionValue& Value)
{
	FVector LookInput = Value.Get<FVector>();

	AddRotator = FRotator(LookInput.Y * CameraSpeedY, LookInput.X * CameraSpeedX, LookInput.Z);
}

```

</details>

<details>
<summary>APlayerPawn::UpdateLook</summary>

```cpp

void APlayerPawn::UpdateLook(float DeltaTime)
{
	if (!AddRotator.IsNearlyZero())
	{
		AddRotator *= (DeltaTime * 20.0f);

		float AddedPitch = SpringArmComp->GetRelativeRotation().Pitch + AddRotator.Pitch;

		// Pitch 회전 각도 제한
		AddedPitch = FMath::Clamp(AddedPitch, -MaxPitchAngle, MaxPitchAngle);

		// 카메라 상하 회전
		SpringArmComp->SetRelativeRotation(FRotator(AddedPitch, 0.0f, 0.0f));

		AddRotator.Pitch = 0.0f;

		// 캐릭터 회전 및 카메라 좌우 회전
		AddActorLocalRotation(AddRotator);
	}

	// 입력 없을시 회전 초기화 -> 회전 정지
	AddRotator = FRotator::ZeroRotator;
}

```

</details>

---

### **중력 및 낙하**

- `AMainPawn::UpdateGravity` 함수로 지속적으로 중력을 적용하고 필요시 재정의 하여 사용한다.
- 캐릭터의 경우 `Sweep`을 사용하여 별도로 바닥을 감지한다.
  > 드론의 경우 회전 및 기울임에 따라 자동으로 `이동` 하는 형태로 작동하기 때문에 `이동`에서 충돌을 감지하여 별도로 바닥을 감지 할 필요는 없다.
- 캐릭터의 경우 공중 이동 배율을 설정하고 적용하여 처음 점프했을 방향과 다른 방향으로 이동시 점차 감소된 이동속도로 이동 하도록 했다.


- **중력 적용 코드 예시**
<details>
<summary>AMainPawn::UpdateGravity</summary>

```cpp

void AMainPawn::UpdateGravity(float DeltaTime)
{
	Velocity.Z += (Gravity * FMath::Pow(DeltaTime, 2));

	// 추락 속도가 중력 가속도를 넘지 않도록 조정
	if (Velocity.Z < Gravity)
	{
		Velocity.Z = Gravity;
	}
}

```

</details>














