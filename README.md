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
## **필수 과제**

- **필수 과제 1번 - C++ Pawn 클래스와 충돌 컴포넌트 구성**

  - `CapsuleComponent`를 `RootComponent`로 만들고 `SkeletalMeshComponent`,`SpringArmComponent`, `CameraComponent`를 부착(`Attach`) 했다.
  - `CapsuleComponent`와 `SkeletalMeshComponent`의 `SetSimulatePhysics(false)`를 호출하여 물리 시뮬레이션을 비활성화 했다.

- **필수 과제 2번 - Enhanced Input 매핑 & 바인딩**

  - 캐릭터 이동
  - `IA_Move` : `WASD`로 입력 받고 축과 입력 방향에 맞춰 모디파이어를 추가 했다.
  
  ![image](https://github.com/user-attachments/assets/40809d9b-fc98-48c6-9c07-c2ea4fc2afe4)

  - `StartMove` : 입력받은 값을 정규화 하여 진행 방향을 얻는다
  - **MainPawn.cpp**
  ```cpp
  
  void AMainPawn::StartMove(const FInputActionValue& Value)
  {
    // 컨트롤러가 없으면 방향 계산 못함
    if (!Controller)
    {
      return;
    }
  
    MoveDirection = Value.Get<FVector>().GetSafeNormal();
  }
  
  ```


  - `StopMove` : 입력이 없으면 

  - 카메라 및 캐릭터 회전
  - `IA_Look` : `마우스 XY 2D 축`을 입력 받고 별도의 모디파이어는 추가하지 않았다.


















