#pragma once
#include"Input.h"
class VibrateManager
{
public:
   VibrateManager();
   static void Initialize();
   static void Finalize();
   static void Update();
   static void SetTime(const float& timer, WORD leftMotor, WORD rightMotor);
private:
    static  float timer_;
    static  WORD leftMotor_ ;
    static  WORD rightMotor_;
};

