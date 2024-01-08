#include <CircleDiagram.hpp>
#include <iostream>

void CircleDiagram::draw(
        sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& i : m_segments)
        target.draw(i);
    target.draw(m_title_text);
}
void CircleDiagram::calculate_name_pos() noexcept
{
    auto font_size = m_title_text.getCharacterSize();
    auto str_len = m_title.size();
    float indent_x = (float)str_len * font_size / 4;
    float indent_y = m_radius + 100;
    sf::Vector2f text_pos = {m_position.x - indent_x, m_position.y - indent_y};

    m_title_text.setString(m_title);
    m_title_text.setFont(font);
    m_title_text.setPosition(text_pos);
    m_title_text.setFillColor(sf::Color::Black);
}

CircleDiagram::CircleDiagram(
        sf::Vector2f position,
        const char* title,
        float radius,
        std::vector<Item>& items)
    : m_title(title), m_position(position), m_radius(radius)
{
    set_items(items);
}

void CircleDiagram::set_font(std::string font_path) noexcept
{
    if (!font.loadFromFile(font_path)) {
        is_font_loaded = false;
        std::cerr << "[ERROR] CircleDiagram: Failed to load font " << font_path
                  << '\n';
    } else {
        is_font_loaded = true;
        calculate_name_pos();
        for (auto& i : m_segments)
            i.set_font(font);
    }
}

void CircleDiagram::set_name(std::string title) noexcept
{
    m_title = title;
    calculate_name_pos();
}

CircleDiagram::CircleDiagram(
        const char* title, float radius, std::vector<Item>& items)
    : CircleDiagram({0, 0}, title, radius, items)
{
}

void CircleDiagram::rotate(float degree) noexcept
{
    if (prev_rotate_degree != degree) {
        float temp = degree;
        degree += 360 - prev_rotate_degree;
        for (auto& i : m_segments) {
            i.rotate(degree);
        }
        prev_rotate_degree = temp;
    }
}

void CircleDiagram::set_items(std::vector<Item>& items)
{
    prev_rotate_degree = 0;
    m_segments.clear();
    std::vector<sf::Color> colors
            = {sf::Color(52, 168, 83),
               sf::Color(251, 188, 5),
               sf::Color(66, 181, 244),
               sf::Color(234, 67, 53),
               sf::Color(160, 160, 160)};

    std::sort(items.begin(), items.end(), [](Item& a, Item& b) {
        return a.get_value() > b.get_value();
    });

    float total_sum = 0;
    float start_angle = 0;

    for (auto& i : items)
        total_sum += i.get_value();

    for (size_t i = 0; i < items.size(); i++) {
        float percent = items[i].get_value() / total_sum;
        float angle_segment = 360 * percent;

        sf::Color color;
        if (i < colors.size())
            color = colors[i];
        else {
            color = colors[i % colors.size()];
            color.r -= i * 5;
            color.g -= i * 5;
            color.b -= i * 5;
            color.a -= i * 5;
        }
        m_segments.push_back(ItemSegment(
                items[i].get_value(),
                items[i].get_title(),
                m_position,
                m_radius,
                angle_segment,
                start_angle,
                color));
        start_angle += angle_segment;
    }

    for (auto& i : m_segments)
        i.rotate(270); // magic number

    if (is_font_loaded) {
        for (auto& i : m_segments)
            i.set_font(font);
    }
}
