#pragma once
#include"Locker.h"
#include<memory>
#include<array>
class Camera;

class LockerManager
{

private:
    std::array < std::unique_ptr<Locker>, 23>lockers1_;
    std::array < std::unique_ptr<Locker>, 23>lockers2_;
public:
    LockerManager();
    void Initialize();
    void Draw(Camera& camera);
    void Update();

    std::array < std::unique_ptr<Locker>, 23>  &GetLockers1() { return lockers1_; };
    std::array < std::unique_ptr<Locker>, 23>  &GetLockers2() { return lockers2_; };

};

