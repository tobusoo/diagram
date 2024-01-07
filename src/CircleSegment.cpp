#include <cmath>

#include <CircleSegment.hpp>

void CircleSegment::draw(
        sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(body);
}

CircleSegment::CircleSegment(
        sf::Vector2f position,
        float radius,
        float segment,
        float start_angle,
        sf::Color color,
        size_t points)
    : m_radius(radius), m_segment(segment), m_points(points)
{
    body.setPosition(position);
    body.setFillColor(color);
    body.setPointCount(m_points + 2);

    body.setPoint(0, {0, 0});
    body.setPoint(1, {m_radius, 0});
    for (size_t i = 1; i < m_points + 1; ++i) {
        float degree = (segment / points * i);
        float radian = degree * M_PI / 180;

        float x = radius * cos(radian);
        float y = radius * sin(radian);
        body.setPoint(i + 1, {x, y});
    }

    rotate(start_angle);
}

void CircleSegment::rotate(float angle) noexcept
{
    float rad = angle * M_PI / 180;
    size_t points = body.getPointCount();
    auto start_point = body.getPoint(0);
    float x0 = start_point.x;
    float y0 = start_point.y;

    for (size_t i = 0; i < points; ++i) {
        auto point = body.getPoint(i);
        float x = (point.x - x0) * cos(rad) - (point.y - y0) * sin(rad) + x0;
        float y = (point.x - x0) * sin(rad) + (point.y - y0) * cos(rad) + y0;

        point = {x, y};
        body.setPoint(i, point);
    }
}
