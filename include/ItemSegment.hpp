#pragma once

#include <SFML/Graphics.hpp>

#include <CircleSegment.hpp>
#include <Item.hpp>

class ItemSegment : public CircleSegment, public Item {
protected:
    sf::Text m_percent_text;
    sf::Text m_title_text;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void calculate_text_pos();

public:
    ItemSegment(
            float value,
            std::string& title,
            sf::Vector2f position,
            float radius,
            float segment,
            float start_angle,
            sf::Color color = sf::Color::Green,
            size_t points = 80);

    void set_font(sf::Font& font);
    void rotate(float angle) noexcept;
};