---
layout: single
title:  "Git TIL 메모 (230722)"
categories: git
tag: [github]
---

※ 본 내용은 얄코 님의 유료 강의를 수강하고 개인적으로 추가 조사하며 공부 목적으로 정리한 글입니다.  
(출처: [원본 인프런 강의](https://www.inflearn.com/course/%EC%A0%9C%EB%8C%80%EB%A1%9C-%ED%8C%8C%EB%8A%94-%EA%B9%83#))  
　  

## 파일 삭제, 이동 명령어
- `git rm (파일명)` 을 사용하면 해당 파일 삭제 후 자동으로 stage 됨
    - 파일을 물리적으로 삭제하면 stage 되지 않기 때문에 `git add .`을 해줘야 함
    - 두 방법 모두 다른 차이는 없으니 편한대로 사용!
- 마찬가지로 `git mv (이전 파일명) (새로운 파일명)` 을 사용하면 파일 이름 변경 후 자동 stage 됨. 나머지 특징도 위와 같음
    - 파일명에 경로까지 적으면 경로 이동도 수행 (rename 과 동시에 가능)
	- 단, 이동할 디렉토리가 미리 존재해야함. `mkdir` 등으로 폴더를 만들고 옮기자.
	- 디렉토리가 없는 곳을 지정할 경우 오류 문구: `fatal: destination directory does not exist`
　  

## reset 세가지 특징 정리
- `--soft`는 커밋만 취소하고 수정사항은 그대로 stage 된 상태로 남겨둔다.
- `--mixed`는 커밋 취소 후 수정사항도 stage에서 내리고 로컬에만 남겨둔다.
- `--hard`는 수정 사항을 모조리 취소하여 로컬까지 싹 원래대로 돌려놓는다.
	- 변경 사항이 아예 사라지니까 신중하게 써야할 듯
　  

## HEAD
- 내가 특정 커밋 지점에서의 상태를 로컬에서 확인하고 싶을 때 사용 (퍼포스의 get revision 역할)
- 사용 명령어 
	- `git checkout HEAD^` (^ 뒤에 숫자 입력 또는 ^갯수만큼 현 위치에서 뒤로 이동함)
	- `git checkout HEAD~` (~ 뒤에 숫자 입력 또는 ~갯수만큼 현 위치에서 뒤로 이동함)
	- `git checkout (commit 해시)` (해당 커밋으로 이동)
	- HEAD 는 소문자 입력  가능
- 원하는 커밋 위치에 HEAD 라는 가상의 임시 브랜치를 만들어 이동하는 개념
	- 실제 존재하는 브랜치로 이동하거나 혹은 그 지점에서 새로운 브랜치를 만들어버리면 가상의 HEAD 브랜치는 사라짐
- HEAD 를 활용해 '몇 단계 전으로 롤백' 같은 느낌으로 리셋가능
	- ex. `git reset HEAD~2` (2단계 전으로 리셋)
　  

## 원격 저장소 관련
- git switch 명령어는 로컬 브랜치로만 이동할 수 있음.
	- 원격 브랜치로 이동을 시도했을 때 오류 문구: `fatal: a branch is expected, got remote branch 'origin/main'`
- 원격 브랜치를 따와서 로컬 브랜치를 만들고 싶을 때는 `-t` 또는 `--track` 인자 사용
	- ex. `git switch -t origin/new-remote-branch`
- 로컬 브랜치를 그대로 원격의 새로운 브랜치에 올리고 싶을 때는 `-u` 또는 `--upstream` 인자 사용
	- ex. `git push -u origin/new-local-branch`
- 각 브랜치의 연결을 확인하고 싶으면 `git branch -vv`
