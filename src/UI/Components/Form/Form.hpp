#include <memory>
#include <string>
#include <vector>

#include "Button.hpp"
#include "Textbox.hpp"

class Form
{
private:
    std::string Name;
    std::vector<std::shared_ptr<BaseButton>> Buttons;
    std::vector<std::shared_ptr<Textbox>> Textboxs;

public:
    Form() = default;
    ~Form() = default;
};
