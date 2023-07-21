---
layout: single
title:  "GitHub 관련 잡다한 메모"
categories: git
tag: [github]
---
  
## GitHub 관련
- 협업 시 rebase 사용 주의사항
	- 내 로컬에서 작업한 변경사항을 합칠 때는 rebase 를 쓰면 안됨
	- `git pull --rebase` 처럼 원격 내용을 당겨올 때는 rebase 써도 됨
- `--set-upstream` 인자는 -u 로 축약 사용
- `git switch -t origin/from-remote` 는 원격의 origin/from-remote 라는 브랜치를 가져와서 같은 이름의 브랜치를 로컬에도 만들고 이후로도 계속 해당 브랜치로 연결을 유지하겠다는 뜻
	- 메시지 입력 시 `branch 'from-remote' set up to track 'origin/from-remote'.` 라고 뜸
	- `git push -u` 와 비슷한 개념

　  
## 기타 질문
- (해결) 원격 저장소 이름으로 쓰이는 origin 의 존재 이유가 뭔지?
	- 하나의 리파지토리에 여러 개의 원격 저장소를 둘 수 있음!
	- origin 항목에 다른 이름을 입력하면 됨 (ex. origin2)
