stm32 project
==============
개인적으로 stm32보드로 진행하는 프로젝트 <br>
MCU: STM32F4 <br>
Compiler Tool: STM32CUBEIDE 1.0.2 <br>
Configuration Tool: STM32CUBEMX 5.3.0 <br>
완료된 프로젝트: STM32F4_5MP_OV5642_TCP <br>
                STM32F4_5MP_OV5642_RTOS <br>
진행중인 프로젝트:  STM32F4_Lepton_2.5_RTOS Project

## STM32F4_Lepton_2.5_RTOS Project

Task Block Diagram

![image](https://user-images.githubusercontent.com/58902883/121890549-70019c00-cd55-11eb-929b-34ebbe9e1f25.png)

IP Address: DHCP 사용 <br>
Netmask address: DHCP 사용 <br>
Gateway Address: DHCP 사용 <br>

간략한 Task 설명

Console Task: 콘솔에서 보내는 명령어 관리
EthernetAPP Task: Client 로 동작하며 Server 명령어 실행
Lepton Task: Lepton 설정 및 관리
TempDetection Task: 이미지 프레임을 검사하여 threshold 넘는 온도 값이 있을 시 알람 발생

추후 업데이트...

## STM32F4_5MP_OV5642_TCP Project
IP Address: 192.168.001.111 <br>
Netmask address: 255.255.255.000 <br>
Gateway Address: 192.168.001.001 <br>
사진 전송 명령어: capture01  <br>
카메라 인터페이스 확인 명령어: cmt1 <br>

## stm32F4_5MP_OV5642_USB Project
Update ... Window 10에서 USB Driver 문제로 중지


## stm32F4_5MP_OV5642_RTOS Project
IP Address: 192.168.001.111 <br>
Netmask address: 255.255.255.000 <br>
Gateway Address: 192.168.001.001 <br>
사진 전송 명령어 : CMRA  <br>
카메라 인터페이스 확인 명령어 :CMIT  <br>
최종 구현 완료



