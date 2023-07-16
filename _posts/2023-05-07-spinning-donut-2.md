---
layout: single
title:  "Spinning Donut 구현 (2)"
categories: coding
---

# Sphere 구현

## 선행 작업
구현을 시작하기에 앞서 우선 편한 구현을 위해 선행 작업을 한다.  

1. 빛의 밝기에 따라 문자 고르기  
  
```cpp
char GetPixel(float light)
{
    float Interval = 1.f;

    for (int i = 0; i < 13; i++)
    {
        if (light > Interval * (float)i)
            continue;
        return pixel[i];
    }

    return pixel[12];
}
```
문자는 공백 제외 12가지로 정해져 있어서 빛 표현 범위만 정해주면 되는데, 간단하게 각 구간별 차이를 1로 정했다.  

2. 3차원 벡터 클래스 타입 정의 및 편의성 함수  
  
```cpp
typedef tuple<float, float, float> myVector;

bool NormalizeVector(myVector& OutVector)
{
    float x = get<0>(OutVector);
    float y = get<1>(OutVector);
    float z = get<2>(OutVector);

    float vecSize = sqrt(x * x + y * y + z * z);
    if (vecSize <= 0.f)
        return false;
    
    OutVector = { x / vecSize, y / vecSize, z / vecSize };
    return true;
}

float DotProduct(const myVector& A, const myVector& B)
{
    float x = get<0>(A) * get<0>(B);
    float y = get<1>(A) * get<1>(B);
    float z = get<2>(A) * get<2>(B);

    return x + y + z;
}
```
매번 튜플을 타이핑하기 귀찮아서 타입을 만들고 자주 쓸 것 같은 정규화와 내적 함수를 만들었다.  

## 그리기 세팅

이제 본격적인 구현인데, 아무래도 첫 시작이라 어떻게 그래픽스를 표현할지 살짝 고민됐다.  
만약 관찰자(=카메라) 위치까지 정해서 표현하는 것, 즉 Perspective view로 그리는 것이 복잡하거나 큰 스케일에서는 더 좋겠지만, 지금은 Orthogonal view 로 단순 투영 형태로 표현한다.  
또한 격자 형태로 일종의 도트 그림처럼 표현하는 것이므로 각 격자의 간격과 표현 범위 등을 정한 후 반복문을 통해 그렸다.  

![image](https://jm911.github.io/assets/images/230507/1.png)  

그냥 구체 하나가 원점에 있는 것을 생각하고 Ambient만 구현하여 옆에서 바라본 모습을 그린 것이다.  
좌우로 찌그러진 건 콘솔 창에서 문자 하나의 높이가 너비보다 더 커서 그렇다.  
여기서 좀 당황스러웠는데 다행히 크기가 2배 차이에 가까웠는지, 그냥 문자마다 공백을 하나씩 더 끼워넣으니까 비슷해졌다.

![image](https://jm911.github.io/assets/images/230507/2.png)  


## Diffuse 구현

먼저 빛의 방향을 벡터로 정해놓고 Phong 이론을 토대로 특정 점에서 도형의 노멀 벡터와 연산을 통해 반사량을 계산한다.  
구체 형태라 노멀 벡터를 구하기 쉬워 계산 자체는 엄청 간단했다. (연습 구현으로 큐브를 선택하지 않은 이유이기도 하다)  

```cpp
int main()
{
    // 빛 - 강도 * 계수를 하나로
    float amb = 1.f;
    float dir = 10.5f;
    myVector dirVec = { 1.f, -1.f, 1.f };
    NormalizeVector(dirVec);
    //float Poi = 1.f;

    // 격자
    float grid = 1.f;
    float range = 21.f; // x,y 방향으로 -range ~ range 까지 그림

    // 도형
    float radius = 20.f;


    // main
    float curX = -range;
    float curY = range;
    float maxX = range + grid * 0.5f;
    float maxY = range + grid * 0.5f;

    while (curY > -maxY)
    {
        while (curX < maxX)
        {
            // 1. ambient 계산
            float curAmb = (curX * curX + curY * curY < radius * radius) ? amb : 0.f;

            // 2. directional 계산
            float curDir = 0.f;

            if (curAmb > 0.f)
            {
                float curZ = -sqrt(radius * radius - curX * curX - curY * curY);
                myVector curNormalVec = { curX, curY, curZ };
                NormalizeVector(curNormalVec);

                curDir = -DotProduct(dirVec, curNormalVec) * dir;
                if(curDir < 0.f)
                    curDir = 0.f;
            }

            // 3. Total
            float totalLight = curAmb + curDir;

            cout << GetPixel(totalLight) << ' ';

            curX += grid;
        }
        cout << '\n';
        curX = -range;
        curY -= grid;
    }
}
```
사실 코드보다는 그림으로 설명하고 싶지만 태블릿 같은 것도 없고, 그렇다고 그림판으로 하기에는 너무 조잡해 보일 것 같고 귀찮아서 생략한다. :cry:  
  
참고로 Directional Light 라는 네이밍이 편해서 이를 토대로 변수명을 지었다.  
curZ 값을 계산할 때 -가 붙는 이유는 사실 카메라가 보는 방향이 Z 방향으로 양수 방향이면 좋을 것 같다는 말도 안되는 생각이 들어서 그렇게 구현했기 때문이다...  
정작 계산할 때 귀찮기만 해서 의미가 없었다.  
그리하여 최종 결과물은 다음과 같다.

![image](https://jm911.github.io/assets/images/230507/3.png)  

생각보다 그럴싸하게 나와서 좋았지만 조금 어색한 점이 있어서 빛의 방향을 비스듬하게 바꿨고, 그리드를 약간 더 촘촘하게 했다.  

```cpp
// ...
    myVector dirVec = { 0.5f, -0.7f, 1.2f };

    float grid = 0.8f;
// ...
```

![image](https://jm911.github.io/assets/images/230507/4.png)  

그리드를 0.5 까지는 줄이고 싶었는데, 콘솔창이 줄이 넘어가버려서 0.8 까지 밖에 줄이지 못했다.  
아까 높이, 너비 이슈도 그렇고 뭔가 해결할 수 있는 설정이 있을 것 같긴 한데 귀찮아서 그냥 쓰기로 했다.  
  
마지막으로 폰트 때문인지 혹은 한 칸을 띄우는 형태라 그런지 몰라도, 뭔가 빛 세기 별 문자 세팅이 살짝 어긋난 느낌이 들었다.  
무엇보다 = 문자가 중간에 너무 튀어 보여서 그것만 좀 앞으로 당겨서 최종적인 형태를 완성했다. 

![image](https://jm911.github.io/assets/images/230507/5.png)  

이렇게 놓고 보니 꽤나 만족스러운 모양이 나와 뿌듯했다.  
그러나 아직 도넛 모양과 움직임까지 구현하려면 갈길이 멀다.  



