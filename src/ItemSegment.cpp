#include <iostream>

#include <ItemSegment.hpp>
#include <sstream>

void ItemSegment::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(body);
    target.draw(m_percent_text);
    target.draw(m_title_text);
}

ItemSegment::ItemSegment(
        float value,
        std::string& title,
        sf::Vector2f position,
        float radius,
        float segment,
        float start_angle,
        sf::Color color,
        size_t points)
    : CircleSegment(position, radius, segment, start_angle, color, points),
      Item(value, title)
{
    std::stringstream percent_str;
    percent_str.setf(std::ios::fixed);
    percent_str.precision(1);
    percent_str << segment * 100 / 360 << '%';
    auto str = percent_str.str();
    m_percent_text.setString(str);

    m_title_text.setString(title);
    m_title_text.setFillColor(sf::Color::Black);

    calculate_text_pos();
}

void ItemSegment::set_font(sf::Font& font)
{
    m_percent_text.setFont(font);
    m_title_text.setFont(font);
}

void ItemSegment::calculate_text_pos()
{
    auto position = body.getPosition();

    auto medium_point = body.getPoint(m_points / 2 + 2);
    float x = position.x + medium_point.x / 1.4;
    float y = position.y + medium_point.y / 1.4;
    auto font_size_p = m_percent_text.getCharacterSize();
    auto font_size_t = m_title_text.getCharacterSize();
    auto title_len = m_title.size();

    x -= font_size_p;
    y -= font_size_p;
    m_percent_text.setPosition({x, y});

    x = position.x + medium_point.x * 1.1;
    y = position.y + medium_point.y * 1.1 - font_size_t / 2;

    if (medium_point.x >= 0 && medium_point.y >= 0) {
        // std::cout << "4: ";
    } else if (medium_point.x <= 0 && medium_point.y >= 0) {
        // std::cout << "3: ";
        x -= title_len * font_size_t / 2;
    } else if (medium_point.x <= 0 & medium_point.y <= 0) {
        // std::cout << "2: ";
        x -= title_len * font_size_t / 2;
    } else {
        // std::cout << "1: ";
    }

    m_title_text.setPosition({x, y});
}

void ItemSegment::rotate(float angle) noexcept
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

    calculate_text_pos();
}
