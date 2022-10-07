#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_SERVICES_CHOOSER_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_SERVICES_CHOOSER_H

#include "game/interface/services_chooser_interface.h"

class ServicesChooser : public ServicesChooserInterface {
public:
    std::vector<std::unique_ptr<ServiceInterface>> getServices() override;

};

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_SERVICES_CHOOSER_H
