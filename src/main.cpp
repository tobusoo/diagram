#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Graphics.hpp>

#include <CircleDiagram.hpp>
#include <CircleSegment.hpp>
#include <ItemSegment.hpp>

#include <cxxopts.hpp>

#ifdef _WIN32
#include <windows.h>

#include <commdlg.h>

char* get_filename_from_user()
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = new char[MAX_PATH];
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";

    if (GetOpenFileName(&ofn) == TRUE) {
        return ofn.lpstrFile;
    } else {
        return NULL;
    }
}

#endif // _WIN32

void screenshot(
        sf::RenderWindow& window, CircleDiagram& diagram, const char* filename)
{
    window.clear(sf::Color(200, 200, 200));
    window.draw(diagram);

    sf::Texture texture;
    if (!texture.create(window.getSize().x, window.getSize().y)) {
        std::cerr << "[ERROR] screenshot: Failed to create texture\n";
        return;
    }

    texture.update(window);
    if (texture.copyToImage().saveToFile(filename))
        std::cout << "[INFO] screenshot saved to " << filename << '\n';
}

void remove_item(std::vector<Item>& items, const char* title)
{
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it).get_title() == title) {
            items.erase(it);
            return;
        }
    }
}

void erase_space(std::string& str)
{
    if (str[0] != ' ')
        return;
    size_t i = 0;
    for (; i < str.size(); i++) {
        if (str[i] == ' ')
            break;
    }

    if (i != str.size() - 1) {
        size_t j = str.find_first_not_of(' ', i);
        str.erase(i, j);
    }
}

void parse_file(std::string filename, std::vector<Item>& items)
{
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] parse_file: Failed to open " << filename << '\n';
        return;
    }
    std::string line;
    std::string title;
    float value = 0;
    items.clear();

    while (std::getline(file, line, '\n')) {
        erase_space(line);
        size_t end_title = line.find(' ');
        title = line.substr(0, end_title);
        try {
            value = std::stof(line.substr(end_title));
        } catch (std::exception& e) {
            continue;
        }
        items.push_back(Item(value, title));
    }
}

void imgui_create_table(std::vector<Item>& items)
{
    ImGui::SetNextWindowPos({0, 255});
    ImGui::SetNextWindowSize({200, 800 - 255});

    ImGui::Begin("Table");
    ImGuiTabBarFlags flags = ImGuiTableFlags_Borders;
    flags |= ImGuiTableFlags_Resizable;

    if (ImGui::BeginTable("Items", 2, flags)) {
        ImGui::TableSetupColumn("Title");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();
        for (size_t row = 0; row < items.size(); row++) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", items[row].get_title().c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.2f", items[row].get_value());
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

int change_diagram_name(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
        auto diagram = (CircleDiagram*)data->UserData;
        diagram->set_name(data->Buf);
    }

    return 0;
}

int main(int argc, char* argv[])
{
    std::vector<Item> items;

    cxxopts::Options options("diagram");
    options.add_options()(
            "i,input", "Path to input file", cxxopts::value<std::string>())(
            "n,name",
            "Diagram name",
            cxxopts::value<std::string>()->default_value("Diagram name"))(
            "h,help", "Print usage");
    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    } catch (cxxopts::exceptions::exception& e) {
        std::cout << e.what() << '\n';
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result.count("input")) {
        parse_file(result["input"].as<std::string>(), items);
    }

    std::string default_diagram_name;
    default_diagram_name = result["name"].as<std::string>();

    sf::ContextSettings sett;
    sett.antialiasingLevel = 8;
    sf::VideoMode mode(1200, 800);
    sf::RenderWindow window(mode, "Circle Diagram", sf::Style::Default, sett);
    window.setFramerateLimit(60);
    window.setPosition({400, 0});

    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Failed to init ImGui window\n";
        exit(EXIT_FAILURE);
    }

    const float radius = 300;
    CircleDiagram diagram(
            {700, 400}, default_diagram_name.c_str(), radius, items);

    char title[100] = {0};
    char diagram_name[100] = {0};
    strcpy(diagram_name, default_diagram_name.c_str());
    float value = 0;
    int degree = 0;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            default:
                break;
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({200, 255});
        ImGui::Begin("Options");
#ifdef _WIN32
        if (ImGui::Button("Select input file")) {
            char* filename = get_filename_from_user();
            if (filename) {
                items.clear();
                parse_file(filename, items);
                diagram.set_items(items);
            }
        }
#endif
        ImGui::Text("Diagram name:");
        ImGui::InputText(
                "name",
                diagram_name,
                IM_ARRAYSIZE(diagram_name),
                ImGuiInputTextFlags_CallbackEdit,
                change_diagram_name,
                (void*)&diagram);

        ImGui::Text("Rotate diagram:");
        ImGui::SliderInt("rotate", &degree, 0, 360);
        ImGui::Text("Enter Title and Value");
        ImGui::InputText("Title", title, IM_ARRAYSIZE(title));
        ImGui::InputFloat("Value", &value);
        if (ImGui::BeginTable("Items", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Button("Add")) {
                if (!(value == 0 || title[0] == '\0')) {
                    items.push_back(Item(value, title));
                    diagram.set_items(items);
                    value = 0;
                    memset(title, 0, IM_ARRAYSIZE(title));
                }
            }
            ImGui::TableSetColumnIndex(1);
            if (ImGui::Button("Remove")) {
                if (title[0] != '\0') {
                    remove_item(items, title);
                    diagram.set_items(items);
                    memset(title, 0, IM_ARRAYSIZE(title));
                }
            }
            ImGui::EndTable();
        }

        if (ImGui::Button("Screenshot"))
            screenshot(window, diagram, "diagram.png");
        ImGui::End();

        imgui_create_table(items);

        diagram.rotate(degree);

        window.clear(sf::Color(200, 200, 200));
        window.draw(diagram);
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
