---
layout: single
title:  "블로그 리파지토리를 Private 으로 변경 시도(실패)"
categories: blog
tag: [github]
toc: true
---

최근 다시 블로그 포스팅에 의욕이 생겨 미뤄왔던 걸 하려고 했다.  
현재 블로그의 깃허브 리파지토리를 Private 으로 변경하는 것인데, 사실 지금은 별거 없지만 이런 작업은 보통 미뤘다가 하면 좋은 꼴을 못보기 때문에 바로 시도해봤다.  
결과적으로 실패했는데, Private 으로 바꾸려면 아래 두 가지 조건이 걸림돌이었다.  

1. Fork 한 리파지토리는 Public 으로만 놔둬야 한다.
2. 깃허브 페이지 기능을 사용하면서 Private 리파지토리를 쓰려면 Pro 요금제를 사용해야 한다.

2번 이유 때문에 막히긴 했지만 그래도 추후에 결제만 하면 간단하게 바꿀 수 있기 때문에 1번을 해결하는 과정을 간단하게 요약해봤다.  
참고로 전체 과정은 아래 스택 오버플로우 링크의 도움을 받았다.  
- [https://stackoverflow.com/questions/10065526/github-how-to-make-a-fork-of-public-repository-private](https://stackoverflow.com/questions/10065526/github-how-to-make-a-fork-of-public-repository-private)

## 1. 리파지토지 정리
기존 블로그 repo 의 이름을 바꾸고, 새로운 repo 를 만들어 놓는다.  
할 일을 요약하면 새로 만드는 repo 에 기존 repo 내용을 모조리 때려넣어서 Fork 에 의한 Visibility 세팅 제한을 회피할 것이다.  

굳이 이름을 바꿔야 하나? 싶지만 처음 블로그 제작할 때 repo 이름을 `~~.github.io` 라고 지어야 한다는 기억이 어렴풋이 나서...  
어쨌든 나는 새로 만드는 repo를 기존 repo 이름으로 사용하고 싶기 때문에 별 생각 없이 변경했다.  

- 기존 블로그 repo 이름 변경 (`jm911.github.io` >> `jm911.github.io_Fork`)
- 새로운 repo 생성 (`jm911.github.io`)

## 2. 리파지토리 내용물 옮기기
내용물은 모두 로컬을 통해 옮길 예정이므로 작업하고 싶은 폴더에 가서 아래 명령어를 입력한다.  

```
git clone --bare (Fork 한 기존 repo 주소)   // 기존 repo 를 bare clone 한다.
cd (기존 repo 로컬 폴더)                    // 위에서 받아온 repo 의 로컬 폴더로 이동
git push --mirror (새로 만든 repo 주소)     // 받아놓은 기존 repo 내용물을 새로운 repo 로 push
```

작업이 종료되면 clone 한 폴더는 지워도 된다.  
`rm -rf (기존 repo 로컬 폴더)`

위 명령어를 내가 사용한 예시는 이렇다.  
참고로 나는 한 줄 씩 입력했다.  
```
git clone --bare https://github.com/JM911/jm911.github.io_Fork.git
cd jm911.github.io_Fork
git push --mirror https://github.com/JM911/jm911.github.io.git
rm -rf jm911.github.io_Fork
```

사실 여기까지 하면 그냥 Fork 해놓은 리파지토리를 옮기는 작업은 모두 끝났다.  
확인해보면 알겠지만 commit 기록까지 싹 다 옮겨진다.  
이후 기존 repo를 삭제하거나 archive 하면 되고, 새로운 repo는 private 으로 바꾸거나 하면 된다.  
(물론 블로그 사용할 거라면 유료 계정이어야만 가능)  

## 3. 새로운 리파지토리 Github Page 세팅
원래 목표했던 visibility 는 못바꿨지만 그래도 블로그로는 사용해야 하기 때문에 세팅을 다시 해준다.  
이런 작업은 보통 처음 만들 때 한 번 해놓고 잊어버리기 때문에 까먹기 십상이라 함께 적어놓는다.  

- 새로운 repo 의 setting 으로 이동하여 Pages 탭 - Branch 를 'None' 에서 'master' 로 변경  
![image](https://jm911.github.io/assets/images/240305/1.png){: width="100%" height="100%" .align-center}

이후 원래 블로그 주소로 들어가면 다시 정상적으로 작동하는 것을 확인할 수 있다.  

