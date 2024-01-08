#pragma once

#include <SFML/Graphics.hpp>

class CircleSegment : public sf::Drawable {
protected:
    sf::ConvexShape body;
    float m_radius;
    float m_segment; // must be from 0 to 360 degrees
    size_t m_points;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    CircleSegment(
            sf::Vector2f position,
            float radius,
            float segment,
            float start_angle,
            sf::Color color = sf::Color::Green,
            size_t points = 80);

    void rotate(float angle) noexcept;
};