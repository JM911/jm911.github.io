#include <iostream>
#include <tuple>
#include <cmath>
#include <Windows.h>
#include <vector>

#define PI 3.14159

using namespace std;
typedef tuple<float, float, float> myVector;

char pixel1[13] = { ' ', '.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@' };
char pixel2[13] = { ' ', '.', '.', '.', ',', ',', ',', 'o', 'o', 'o', '0', '0', '0' };

char GetPixel(float light);
bool NormalizeVector(myVector& OutVector);
float DotProduct(const myVector& A, const myVector& B);

int main()
{
    // 빛 - 강도 * 계수를 하나로
    float amb = 1.f;
    float dir = 10.5f;
    float dirVecTheta = 0.f;
    myVector dirVec = { cos(dirVecTheta), -1.f, sin(dirVecTheta)}; //{ 0.5f, -0.7f, 1.2f };
    NormalizeVector(dirVec);
    //float Poi = 1.f;

    // 격자
    float gridX = 0.4f;
    float gridY = 0.8f;
    float range = 21.f; // x,y 방향으로 -range ~ range 까지 그림

    // 도형
    float radius = 20.f;

    // main
    float curX = -range;
    float curY = range;
    float maxX = range + gridX * 0.5f;
    float maxY = range + gridY * 0.5f;

    // Start Delay
    int startCall;
    cin >> startCall;

    int gridNumX = (int)((range * 2) / gridX) + 2;
    int gridNumY = (int)((range * 2) / gridY) + 2;
    vector<vector<char>> pixelColors(gridNumX, vector<char>(gridNumY, ' '));

    printf("\x1b[2J");    // 화면 전체 지우기
    printf("\x1b[?25l");    // 커서 숨기기
    while (true)
    {
        printf("\x1b[H"); // 커서를 (0, 0)으로 이동
        curX = -range;
        curY = range;
        int curIdxX = 0;
        int curIdxY = 0;

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
                    if (curDir < 0.f)
                        curDir = 0.f;
                }

                // 3. Total
                float totalLight = curAmb + curDir;

                pixelColors[curIdxX][curIdxY] = GetPixel(totalLight);
                //cout << GetPixel(totalLight);

                curX += gridX;
                ++curIdxX;
            }
            //cout << '\n';

            curX = -range;
            curIdxX = 0;

            curY -= gridY;
            ++curIdxY;
        }

        // 출력
        //system("cls");
        for (int j = 0; j < gridNumY; ++j)
        {
            for (int i = 0; i < gridNumX; ++i)
            {
                cout << pixelColors[i][j];
            }
            cout << '\n';
        }

        // Update
        //radius += 0.2f;

        dirVecTheta += 0.1f;
        if (dirVecTheta > 2 * PI) dirVecTheta -= 2 * PI;
        get<0>(dirVec) = cos(dirVecTheta);
        get<1>(dirVec) = -1.f;
        get<2>(dirVec) = sin(dirVecTheta);
        NormalizeVector(dirVec);
        

        // Sleep
        Sleep(30);
    }
}

char GetPixel(float light)
{
    float Interval = 1.f;

    for (int i = 0; i < 13; i++)
    {
        if (light > Interval * (float)i)
            continue;
        return pixel2[i];
    }

    return pixel2[12];
}

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
