#include "Button.hpp"

BaseButton::BaseButton() : content(""), isClick(false), isHover(false) {}
void BaseButton::Process()
{
    if (this->OnHover())
        HoverFunc();
    if (this->OnClick())
        ClickFunc();
}
bool BaseButton::OnHover()
{
    return (true /*and mourse hover*/);
}
bool BaseButton::OnClick()
{
    return (OnHover() /*and mourse click*/);
}
void BaseButton::HoverFunc()
{
}
void BaseButton::ClickFunc()
{
}
void BaseButton::SetContent(const std::string &n_Content)
{
    this->content = n_Content;
}
std::string BaseButton::GetContent() const
{
    return this->content;
}
ButtonType BaseButton::GetButtonType() const
{
    return ButtonType::NONE;
}