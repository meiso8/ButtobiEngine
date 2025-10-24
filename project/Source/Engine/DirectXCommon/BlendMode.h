#pragma once
enum BlendMode {
    //ブレンドなし
    kBlendModeNone,
    //通常ブレンド
    kBlendModeNormal,
    //加算
    kBlendModeAdd,
    //減算
    kBlendModeSubtract,
    //乗算
    kBlendModeMultiply,
    //スクリーン
    kBlendModeScreen,
    //カウント用　利用禁止
    kCountOfBlendMode,
};
