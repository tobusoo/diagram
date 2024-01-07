#include <Item.hpp>

Item::Item(float value, std::string title) : m_value(value), m_title(title)
{
}

float Item::get_value() const noexcept
{
    return m_value;
}

std::string& Item::get_title() noexcept
{
    return m_title;
}
