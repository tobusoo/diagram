#pragma once

#include <ItemSegment.hpp>
#include <SFML/Graphics.hpp>

class CircleDiagram : public sf::Drawable {
private:
    sf::Font font;
    sf::Text m_title_text;
    bool is_font_loaded = false;

    std::vector<ItemSegment> m_segments;
    std::string m_title;
    sf::Vector2f m_position;
    float m_radius;

    int prev_rotate_degree = 0;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void calculate_name_pos() noexcept;

public:
    CircleDiagram(
            sf::Vector2f position,
            const char* title,
            float radius,
            std::vector<Item>& items);
    CircleDiagram(const char* title, float radius, std::vector<Item>& items);

    void set_font(std::string font_path) noexcept;
    void set_name(std::string title) noexcept;
    void rotate(float degree) noexcept;
    void set_items(std::vector<Item>& items);
};
