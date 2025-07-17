#include <memory>
#include <string>

class Textbox
{
private:
    std::string label;
    std::string content;
    bool isClick;
    bool isHover;
    bool isForcus;
public:
    Textbox() = default;
    ~Textbox() = default;
    void Process();
    bool OnHover();
    bool OnClick();
    bool OnForcus();
    bool OnChange();
    void HoverFunc();
    void ClickFunc();
    void ForcusFunc();
    void SetLabel(const std::string &n_Label);
    void SetContent(const std::string &n_Content);
    std::string GetContent() const;
};