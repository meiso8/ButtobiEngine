#pragma once

struct HPs {
    int maxHp;
    int hp;
    int hpDecrease;
};

struct CharacterState {
    bool isHit;
    bool isAttack;
};

struct Damage {
    float cannotControlTime;
    float invincibilityTime;
    float flashTimer;
    bool isHit;
    bool isInvincible;
    HPs hps;
    bool isDead;
};
