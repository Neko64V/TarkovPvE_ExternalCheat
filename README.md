## 概要
SPT-AKI 3.9.X 向けの外部チートです。チート開発に対する興味を失ったので公開しておきます。  
モダンなGUIメニューやビジュアルを兼ね備えています。研究、分析、学習や調査用にもどうぞ。

## 機能
* ESP
* Exfil ESP
* Item ESP（Commented & WIP）
* Grenade ESP
* MenuKey : INSERT

## 備考
* CPlayer->Update(); <- 多分この関数がめっちゃCPU喰ってる説ある。めんどいなぁ…。

## 実行
必要なライブラリは全て揃っているので、VisualStudioでビルドすれば普通に動くはずです。  
https://learn.microsoft.com/ja-jp/cpp/build/vscpp-step-0-installation?view=msvc-170

## 免責事項
このプロジェクトは学習や研究・教育用としてアップロードされました。  
これらの用途以外で使用した場合に発生した如何なる損害についても、製作者(Neko64V)は一切の責任を負いません。  

## 使用したライブラリ
* Microsoft DirectXTK (SimpleMath)  
https://github.com/microsoft/DirectXTK  
* ImGui  
https://github.com/ocornut/imgui  

## アップデート
少なくともSPT-AKI 3.9.X に対応しています。Live EFTで使用するにはオフセットを更新する必要があります。  
GameObjectManagerは滅多に更新されないので基本的にはオフセットのアップデートのみ必要です。  
CheatEngine ->Mono -> .Net Info -> Assembly-CSharp -> EFT.GameWorld / EFT.Player
![image](https://github.com/user-attachments/assets/97fe9626-4269-443e-97a6-9cc145b6d698)

## スクリーンショット
![image](https://github.com/user-attachments/assets/0be7e79e-6d53-4963-9b5f-2a2ddf21d969)
![image](https://github.com/user-attachments/assets/849a1807-e477-4478-92be-08cb723c9b0c)
