---
layout: single
title:  "랜덤 맵 생성 로직 설명"
categories: coding
tag: [unreal]
toc: true
---

![image](https://jm911.github.io/assets/images/240417/1.gif){: .align-center}  

# 들어가기에 앞서
- 본 기능은 "엔터 더 건전" 게임의 맵 생성 로직 중 일부를 언리얼 엔진으로 구현한 것입니다.  
![image](https://jm911.github.io/assets/images/240417/1.jpg){: .align-center}  
- 큰 단위에서의 로직만 구현했으므로 실제 게임에 사용하기 위해서는 디테일한 조정이 더 필요합니다.

# 개요
- 바인딩 오브 아이작, 엔터 더 건전, 하데스, 던전 오브 엔드리스 등 많은 로그라이크 게임에서는 '방' 단위의 아주 작은 전투 씬을 여러 개 묶어 하나의 스테이지로 구성합니다.
- 이 때 어떤 방이 나오는지, 방 사이의 배치 연결이 어떻게 이루어지는지 등의 요소를 랜덤하게 생성하고, 이는 매 플레이마다 변수를 만들고 긴장을 유발하는 중요 요소로 작동합니다.
- 본 기능에서는 이런 기능 중 일부를 모방하여 언리얼 엔진에 구현해보았습니다.


# 클래스
- 참고: 클래스 명의 접두어 LAB는 프로젝트 이름입니다.

## 방 (LABRoomBase)

### 목적 및 특징
- 방마다 원하는 모양의 프리셋을 만들고 BP 형태로 저장하여 그것을 그대로 사용할 수 있도록 기반 제작
- 방의 크기부터 안쪽 내용물까지 직접 지정하여 배치할 수 있어야 하고, 이 상태 그대로 인게임에서 스폰되어야 합니다.
- 생성 시 방들 간에 필요한 함수들 내장 (위치별 충돌, 통로 연결 등)
- Actor 클래스 상속
  
### 기능 별 설명
- 인스턴스 메쉬를 사용하여 원하는 크기의 직사각형 모양 바닥을 자동 생성하고 벽도 정해진 높이만큼 생성
    - 테스트에 사용한 4종류의 방 프리셋 예시
    ![image](https://jm911.github.io/assets/images/240417/1.png){: .align-center}  
    - 구조가 직사각형이라 맵 모양의 자유도가 낮지만, 추후 비활성화 영역을 설정할 수 있게 하여 ┼, ┌ 모양 같은 방도 생성할 수 있게 하는 것을 고려 중입니다.
- 외부에서 방의 위치를 정해주면 방의 바닥 중앙을 기준으로 배치
- 현재 위치에서 지정된 다른 방과 충돌 여부를 계산하여 반환

    ```cpp
    bool ALABRoomBase::IsCollideWith(const ALABRoomBase* OtherRoom)
    {
        const float diffX = FMath::Abs(CenterLocation.X - OtherRoom->CenterLocation.X);
        const float diffY = FMath::Abs(CenterLocation.Y - OtherRoom->CenterLocation.Y);
        // 최소 간격만큼 벌어져있지 않으면 직접 닿지 않아도 충돌로 판정
        const float eps = 100.f;

        const float myX = (float)FloorNumX * FloorSize * 0.5f + WallSize;
        const float myY = (float)FloorNumY * FloorSize * 0.5f + WallSize;
        const float otherX = (float)(OtherRoom->FloorNumX) * (OtherRoom->FloorSize) * 0.5f + OtherRoom->WallSize;
        const float otherY = (float)(OtherRoom->FloorNumY) * (OtherRoom->FloorSize) * 0.5f + OtherRoom->WallSize;
        
        const bool bCollideX = diffX < (myX + otherX + eps);
        const bool bCollideY = diffY < (myY + otherY + eps);

        return bCollideX && bCollideY;
    }
    ```
- 정해진 방향에 벽에 출입용 공간 생성
    - 벽을 인스턴스 메쉬를 통한 큐브의 조합으로 만들기 때문에 원하는 위치를 비우고 다시 벽을 생성하면 적절하게 구멍을 뚫을 수 있습니다.
    - 또한 스케일 조정을 통해 문의 크기도 세부적으로 조절할 수 있습니다.  
    
    ```cpp
    void ALABRoomBase::MakeDoor(EAdjacentDirection Direction)
    {
        DoorDirectionSet.Add(Direction);
        RefreshWall();
    }

    void ALABRoomBase::RefreshWall()
    {
        InstancedWall->ClearInstances();
        InstancedWall->SetStaticMesh(WallMesh);

        TArray<EAdjacentDirection> Directions = { EAdjacentDirection::UP, EAdjacentDirection::LEFT, 
                                                EAdjacentDirection::DOWN, EAdjacentDirection::RIGHT };
        
        if(WallMesh)
        {
            for(auto dir : Directions)//int dir=0; dir<4; dir++)
            {
                // 문 생성 방향 (위치는 일단 가운데에 생성, 크기는 멤버 변수 고정 => 추후 변경 여지 있음)
                int DoorWidthStartIndex = -1;
                int DoorWidthEndIndex = -1;
                int DoorHeightEndIndex = -1;	// 항상 바닥부터 시작하므로 시작 인덱스는 필요 없음
                if(DoorDirectionSet.Contains(dir))
                {
                    float DoorStartDist = (GetWidthLength(false) - DoorWidth) / 2.f;
                    if(dir == EAdjacentDirection::LEFT || dir == EAdjacentDirection::RIGHT)
                        DoorStartDist = (GetHeightLength(false) - DoorWidth) / 2.f;
                    
                    float DoorEndDist = DoorStartDist + DoorWidth;

                    DoorWidthStartIndex = DoorStartDist / WallSize; 
                    DoorWidthEndIndex = DoorEndDist / WallSize;
                    DoorHeightEndIndex = DoorHeight / WallSize;
                }
                
                ...
                
                for(int i=0; i<WallLength; i++)
                {
                    for(int j=0; j<WallHeight; j++)
                    {
                        // 문 위치라면 스킵 - TODO: 문 끝자락은 없애지 않고 스케일, 위치 오프셋 변경하도록 (문 크기 디테일하게 조정)
                        if(i >= DoorWidthStartIndex && i <= DoorWidthEndIndex && j < DoorHeightEndIndex)
                            continue;
                        
                        ...
                    }
                }
            }
        }
    }
    ```
- 현재 위치에서 다른 방까지 연결된 통로 생성
    - 통로가 이어주는 두 개의 방 중 하나를 골라 반대편까지 통로 메쉬를 만들도록 하며, 마찬가지로 인스턴스 메쉬를 사용했습니다.
    - 후술할 맵 레이아웃이 트리 형태이므로 자식 방 -> 부모 방으로 규칙을 정하여 통로를 생성합니다.

    ```cpp
    void ALABRoomBase::MakePath(EAdjacentDirection Direction, float Length)
    {
        if(Direction == EAdjacentDirection::NONE) return;
        
        float DistanceFromCenter = 0.f;
        FVector PathDirection(0.f);

        switch (Direction)
        {
        case EAdjacentDirection::UP:
            DistanceFromCenter = GetHeightLength(true) * 0.5f;
            PathDirection = FVector(1.f, 0.f, 0.f);
            break;
        case EAdjacentDirection::DOWN:
            DistanceFromCenter = GetHeightLength(true) * 0.5f;
            PathDirection = FVector(-1.f, 0.f, 0.f);
            break;
        case EAdjacentDirection::LEFT:
            DistanceFromCenter = GetWidthLength(true) * 0.5f;
            PathDirection = FVector(0.f, -1.f, 0.f);
            break;
        case EAdjacentDirection::RIGHT:
            DistanceFromCenter = GetWidthLength(true) * 0.5f;
            PathDirection = FVector(0.f, 1.f, 0.f);
            break;
        }

        FVector Offset = FVector(0.f, 0.f, -1.f);
        FVector PathStartLocation = CenterLocation + PathDirection * DistanceFromCenter + Offset;
        FVector PathEndLocation = PathStartLocation + PathDirection * Length;
        FVector PathRightDirection = FVector::CrossProduct(FVector::UpVector, PathDirection);
        
        if(PathSize <= 0.f) return;

        for(int i=-1; i<2; i++)
        {
            float CurLength = 0.f;
            FVector CurOffset = PathRightDirection * i * PathSize;
            UE_LOG(LogTemp, Warning, TEXT("CurOffset - %f, %f, %f"), CurOffset.X, CurOffset.Y, CurOffset.Z);
            while(CurLength < Length)
            {
                FVector InstanceLocation =  PathStartLocation + PathDirection * CurLength + CurOffset;
                InstanceLocation = GetActorTransform().InverseTransformPosition(InstanceLocation);
            
                FTransform InstanceTransform = FTransform::Identity;
                InstanceTransform.SetLocation(InstanceLocation);
                InstancedPath->AddInstance(InstanceTransform);
            
                CurLength += PathSize;
            }
        }
    }
    ```


## 맵 생성 (LABLevelGenerator)
### 목적 및 특징
- 트리 형태의 원하는 맵 레이아웃 입력받고 거기에 맞게 방 생성 및 연결
- 방은 랜덤으로 지정되며, 크기가 모두 다르므로 충돌하지 않게 위치 조정하며 생성
- 일종의 매니저 클래스이므로 언리얼에서 제공하는 Subsystem 타입으로 클래스 제작
    - 그 중에서도 GameInstanceSubsystem을 상속

### 맵 생성 로직
0. 방 생성 전에 랜덤으로 생성할 방 BP 클래스를 모두 미리 등록
1. 첫 방(트리의 부모 노드)을 기본 위치 (현재 0, 0, 0) 에 생성
    - 모든 방은 앞서 등록된 방들 중 랜덤 생성
2. 트리를 DFS 방식으로 탐색하며 다음 방을 생성한다.
    - 생성 방향은 랜덤이고 기존에 연결된 방향이 있다면 반대 방향을 우선시 한다.
    - 이미 생성한 방들과 충돌하지 않을 때까지 해당 방향으로 조금씩 이동시켜 생성 거리를 정한다. 
3. 방을 생성하며 부모 방과 연결된 통로 및 문 생성 처리도 하고, 추가로 부모 방 쪽에도 문을 뚫어준다.

위 로직은 모든 과정이 한 번에 수행되지만 과정을 시각화하기 위해 방 생성 간 딜레이를 주어 아래 GIF로 만들어 보았습니다.

![image](https://jm911.github.io/assets/images/240417/2.gif){: width="30%" height="30%"} 
![image](https://jm911.github.io/assets/images/240417/3.gif){: width="30%" height="30%"} 
![image](https://jm911.github.io/assets/images/240417/4.gif){: width="30%" height="30%"}



