#include <memory>
#include <string>
using namespace std;

enum ButtonType{
    NONE = 0,
    CreateButton = 1,
    DeleteButton = 2,
    SuccessButton = 3
};

class BaseButton
{
private:
    std::string content;
    bool isClick;
    bool isHover;
public:
    BaseButton();
    ~BaseButton() = default;
    virtual void Process();
    virtual bool OnHover();
    virtual bool OnClick();
    virtual void HoverFunc();
    virtual void ClickFunc();
    void SetContent(const std::string &n_Content);
    std::string GetContent() const;
    virtual ButtonType GetButtonType() const;
};