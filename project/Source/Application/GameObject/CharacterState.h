#pragma once

struct HPs {
    int maxHp;
    int hp;
};

struct CharacterState {
    bool isHit;
    bool isAttack;
    HPs hps;
};