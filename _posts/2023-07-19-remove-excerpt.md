---
layout: single
title:  "글 목록에서 내용 미리보기 없애버리기"
categories: blog
tag: [jekyll, minimal-mistakes]
---

![image](https://jm911.github.io/assets/images/230719/1.png){: width="80%" height="80%" .align-center}
　  
블로그에 접속하면 정말 뵈기 싫은 것이 저 포스트 내용 미리보기였다.  
정말 너무 지우고 싶었는데 도저히 검색 키워드를 못찾아서 잊고 있었다.  
그러다 다른 분의 가이드 글을 읽을 때 그 부분에 대해 설명해주신 부분이 있길래 반색하고 읽었는데, 도저히 어디를 설명하는지 이해가 되지 않는 것이다.  
참고 포스팅: [https://ansohxxn.github.io/blog/category/](https://ansohxxn.github.io/blog/category/)  
(부끄럽지만 다시 읽어보면 모두 꼼꼼하게 설명해주셨는데 내가 급하게 읽다가 다른 파일로 착각해 벌어진 일이었다)  
  
그러다 오기가 생겨 마구잡이로 뒤져보다가 방법을 발견하여 기록으로 남긴다.  
　  
## 구글 크롬 - F12
웹 프로그래머들에게는 익숙한 기능이겠지만 내 입장에선 그냥 이런게 있구나 하고 넘긴 건데, 구글 크롬의 웹페이지에서 F12 키를 누르면 해당 화면의 소스 코드를 보여준다.  
내가 이해할 수 없는 코드지만 크롬은 친절하게도 각 코드에 대한 페이지 부분을 색으로 표시까지 해준다!  
　  
![image](https://jm911.github.io/assets/images/230719/2.png)  
　  
이런 식으로 마우스를 해당 코드에 올려 놓으면 친절하게 알려준다.  
딱 내가 보고 싶었던 부분이 excerpt 라는 이름으로 나오는 것을 확인 할 수 있다. (내가 그렇게 찾고 싶던 미리보기 부분에 대한 키워드이다)  
지금까지 preview, entry layout, thumbnail 이런 식으로 모르는 키워드를 마구잡이로 검색했으니 나올리가 없지... 알고 나니 속이 다 뻥 뚫렸다.  
(그런데 보통 구글링하면 비슷한 단어로 치환해주기도 하는데, 이건 좀 마이너 이슈라 그런지 그런 것도 없었다 :disappointed_relieved:)  
  
역시나 'jekyll remove excerpt' 이런 식으로 검색하니 결과도 바로 나왔다.  
파일 쪽은 home layout 이나 index.html 같은 곳에 표기되어 있을 줄 알았는데, 그게 아니라 **includes/archive-single.html** 에 이런 리스트 뷰에 대한 정보가 적혀 있었다.  
  
![image](https://jm911.github.io/assets/images/230719/3.png)  
  
이렇게 excerpt 어쩌구 하는 부분을 주석처리 해주면 모든 archive 레이아웃의 미리보기가 사라진다.  
　  
## 결과
  
![image](https://jm911.github.io/assets/images/230719/4.png)  
  
드디어 정말 너무나도 깔끔해졌다! :tada:  
찾다보니 포스트마다 excerpt 를 머릿말에서 별개로 설정할 수 있는 방법도 있던데, 나중에는 거기에 간단한 키워드나 문장을 넣어서 더욱 보기 좋게 만들 수도 있을 것 같다.  
또한 날짜 표기의 위치나 글자 크기 등도 커스텀하고 싶긴 한데, 일이 커질까봐 우선은 여기에 만족하기로 했다.  
