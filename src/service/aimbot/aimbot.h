#ifndef ASSAULT_CUBE_HACKING_SERVICE_AIMBOT_AIMBOT_H
#define ASSAULT_CUBE_HACKING_SERVICE_AIMBOT_AIMBOT_H

class Aimbot {
    void aimHead(Player &localPlayer, Player &enemy);

public:
    static Aimbot &getInstance();
    void callback();


};

#endif //ASSAULT_CUBE_HACKING_SERVICE_AIMBOT_AIMBOT_H
