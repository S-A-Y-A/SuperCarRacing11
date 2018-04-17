#pragma once

//ゲームのループ(コマンドを送って結果を受け取るの繰り返し)
void Game_Loop();//サーバーにゲームでの毎ターンの情報（キー入力とか）を送る関数


//サーバーから全キャラの情報を受け取る関数　受け取って種類に応じて処理する
void Receive_Game_Status();

void Input_And_Send();

//画面描写する関数　内部で　マップとキャラ　その他のものを描写する
void Draw_Window_Racing();
