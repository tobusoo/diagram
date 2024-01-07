#pragma once
#include <string>

class Item {
protected:
    float m_value;
    std::string m_title;

public:
    Item(float value, std::string title);

    float get_value() const noexcept;
    std::string& get_title() noexcept;
};