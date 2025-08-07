#include "StorageScreen.hpp"

StorageScreen::StorageScreen(Core &core)
: Screen(core){}

void StorageScreen::Init(){

}
void StorageScreen::OnExit(){

}
void StorageScreen::Render(float dt){
    ImGui::Text("Hiulaptop");
}