#include <iostream>
#include <tuple>
#include <cmath>
#include <Windows.h>
#include <vector>

#define PI 3.14159

using namespace std;
typedef tuple<float, float, float> myVector;

char pixel1[13] = { ' ', '.', ',', '-', '~', ':', ';', '!', '*', '=', '#', '$', '@' };
char pixel2[13] = { ' ', '.', '.', '.', ',', ',', ',', 'o', 'o', 'o', '0', '0', '0' };

char GetPixel(float light);
bool NormalizeVector(myVector& OutVector);
float DotProduct(const myVector& A, const myVector& B);

int main()
{
    // 빛 - 강도 * 계수를 하나로
    float amb = 1.f;
    float dir = 11.f;
    float dirVecTheta = 60.f / 180.f * PI;
    myVector dirVec = { cos(dirVecTheta), -1.f, sin(dirVecTheta)}; //{ 0.5f, -0.7f, 1.2f };
    NormalizeVector(dirVec);
    //float Poi = 1.f;

    // 격자
    int gridNumX = 100;
    int gridNumY = 50;
    vector<vector<char>> pixelColors(gridNumX, vector<char>(gridNumY, ' '));
    vector<vector<float>> pixelDepth(gridNumX, vector<float>(gridNumY, 10000.f));

    // 도형
    float radius = 37.5f;
    float br = 22.5f;   // big R
    float sr = 10.f;    // small r
    float brDir = 1.f;
    float srDir = 1.f;

    // 토러스 축 회전
    float A = 0.f, B = 0.f, C = PI / 2.f;

    // Start Delay
    int startCall;
    cin >> startCall;

    printf("\x1b[2J");    // 화면 전체 지우기
    printf("\x1b[?25l");    // 커서 숨기기
    while (true)
    {
        printf("\x1b[H"); // 커서를 (0, 0)으로 이동
        for (int i = 0; i < gridNumX; ++i)
        {
            fill(pixelColors[i].begin(), pixelColors[i].end(), ' ');
            fill(pixelDepth[i].begin(), pixelDepth[i].end(), 10000.f);
        }

        float theta = 0.f, thetaStep = 0.02f;
        float phi = 0.f, phiStep = 0.03f;
        for (theta = 0.f; theta < 2 * PI; theta += thetaStep)
        {
            for (phi = 0.f; phi < 2 * PI; phi += phiStep)
            {
                // 1. rasterize
                float cosTheta = cos(theta);
                float sinTheta = sin(theta);
                float cosPhi = cos(phi);
                float sinPhi = sin(phi);

                float cosA = cos(A), sinA = sin(A);
                float cosB = cos(B), sinB = sin(B);
                float cosC = cos(C), sinC = sin(C);

                float R11 = cosA * cosB, R12 = cosA * sinB * sinC - sinA * cosC, R13 = cosA * sinB * cosC + sinA * sinC;
                float R21 = sinA * cosB, R22 = sinA * sinB * sinC + cosA * cosC, R23 = sinA * sinB * cosC - cosA * sinC;
                float R31 = -sinB, R32 = cosB * sinC, R33 = cosB * cosC;

                float x0 = br * cosPhi + sr * cosTheta * cosPhi;
                float y0 = sr * sinTheta;
                float z0 = br * sinPhi + sr * cosTheta * sinPhi;

                float x = x0 * R11 + y0 * R12 + z0 * R13;
                float y = x0 * R21 + y0 * R22 + z0 * R23;
                float z = x0 * R31 + y0 * R32 + z0 * R33;

                int gridX = (int)(x + 50.f);
                int gridY = (int)((50.f - y) / 2.f);

                // 유효 격자 확인
                if (gridX < 0 || gridX >= gridNumX || gridY < 0 || gridY >= gridNumY) continue;

                // 이미 더 앞쪽 픽셀이 기록되어 있으면 스킵
                if(pixelDepth[gridX][gridY] <= z) continue;

                //pixelColors[gridX][gridY] = '.';
                pixelDepth[gridX][gridY] = z;


                // 2. lighting
                float curAmb = amb;

                float nX0 = cosTheta * cosPhi;
                float nY0 = sinTheta;
                float nZ0 = cosTheta * sinPhi;

                float nX = nX0 * R11 + nY0 * R12 + nZ0 * R13;
                float nY = nX0 * R21 + nY0 * R22 + nZ0 * R23;
                float nZ = nX0 * R31 + nY0 * R32 + nZ0 * R33;

                myVector curNormalVec = { nX, nY, nZ };
                
                float curDir = -DotProduct(dirVec, curNormalVec) * dir;
                if (curDir < 0.f) curDir = 0.f;

                float totalLight = curAmb + curDir;
                pixelColors[gridX][gridY] = GetPixel(totalLight);
            }
        }

        /* 출력 */
        for (int j = 0; j < gridNumY; ++j)
        {
            for (int i = 0; i < gridNumX; ++i)
            {
                cout << pixelColors[i][j];
            }
            cout << '\n';
        }

        /* Update */
        
        // 빛 회전
        //dirVecTheta += 0.1f;
        //if (dirVecTheta > 2 * PI) dirVecTheta -= 2 * PI;
        //get<0>(dirVec) = cos(dirVecTheta);
        //get<1>(dirVec) = -1.f;
        //get<2>(dirVec) = sin(dirVecTheta);
        //NormalizeVector(dirVec);

        // 토러스 축 회전
        A += 0.065f;
        if (A > 2 * PI) A -= 2 * PI;
        B -= 0.1f;
        if (B > 2 * PI) B -= 2 * PI;
        if (B < 0) B += 2 * PI;
        C += 0.135f;
        if (C > 2 * PI) C -= 2 * PI;

        // 토러스 크기 변화
        //br += 0.8f * brDir;
        //if (br > 30.f) brDir = -1.f;
        //if (br < 10.f) brDir = 1.f;
        //sr += 0.5f * srDir;
        //if (sr > 15.f) srDir = -1.f;
        //if (sr < 5.f) srDir = 1.f;
        

        /* Sleep */
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
        return pixel1[i];
    }

    return pixel1[12];
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
