# TetriStrike

Developed with Unreal Engine 5


Shooting Tetris Game

Creating by 1 month of Unreal Engine, 4 people, 2 weeks project Using C++, Blueprint

1. 게임 소개
3D테트리스 게임으로 블록(Mino)가 떨어지고, 일정 이상 블록이 쌓이면 층으로 판단하여 폭파되며 스코어가 증가하는 게임
![image](https://github.com/user-attachments/assets/f5bd70ac-d749-49c2-a019-ce28f3cd6196)

3. 기능
- 정방향(노란 탄환), 역방향(빨간 탄환)을 발사하면 정방향 혹은 역방향 물리적 충격을줌.
- 차지 어택(Charge Attack)으로 플레이어가 발사를 누르고 있는 시간에 비례하여 탄환의 힘과 속도가 증가
- ChekBall 엑터를 레벨에 배치하여 층을 인식하고, 일정 이상 채워지면 폭파하며 블록은 잘림.
  ![CheckBall](https://github.com/user-attachments/assets/e092857d-2959-48ae-855c-a74696a76c0b)
  ![image](https://github.com/user-attachments/assets/d3684273-23f4-49c6-9cf6-faa5071d8ee5)
- Local MultiPlayer로 2P를 구현
![image](https://github.com/user-attachments/assets/86e91f5f-2d39-4a2f-aa63-d6f6faadc497)
- 포탈 탄환으로 생성시켜 블록을 워프 시킴.
![image](https://github.com/user-attachments/assets/47240f05-81f1-47ed-9fb3-387ff8a4ea4e)
