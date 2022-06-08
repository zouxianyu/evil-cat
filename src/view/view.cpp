#include "view.h"
#include "imgui_d3d9_external/view.h"

View &View::getInstance() {
    return ImGuiD3D9ExternalView::getInstance();
}
