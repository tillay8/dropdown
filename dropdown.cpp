#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <array>
#include <string>
using std::string;
class DropdownPanel {
    private:
    sf::RenderWindow window;
    sf::Color backgroundColor, highlightColor;
    int width, height, posX, posY;
    std::vector<std::pair<string, string>> menuOptions;
    const int FONT_SIZE = 16;
    const int SPACING = 20;

    sf::Text createText(const string& text, sf::Font& font, int size, int x, int y) {
        sf::Text menuText;
        menuText.setFont(font);
        menuText.setString(text);
        menuText.setCharacterSize(size);
        menuText.setFillColor(sf::Color::Black);
        menuText.setPosition(x, y);
        return menuText;
    }

    sf::RectangleShape createHighlightRect(int x, int y, int width, int height) {
        sf::RectangleShape rect(sf::Vector2f(width + 20, height + 8));
        rect.setFillColor(highlightColor);
        rect.setPosition(x - 10, y + 4);
        return rect;
    }

    public:
    DropdownPanel(const std::array<int, 4>& dimensions, sf::Color bgColor, sf::Color hlColor = sf::Color(100, 149, 237))
        : posX(dimensions[0]), posY(dimensions[1]),
          width(dimensions[2]), height(dimensions[3]),
          backgroundColor(bgColor), highlightColor(hlColor) {
        window.create(sf::VideoMode(width, height), "dropdown", sf::Style::None);
        window.setPosition(sf::Vector2i(posX, posY));
    }
    
    void addMenuOption(const string& text, const string& command) {
        menuOptions.emplace_back(text, command);
    }

    void addSpacer() {
        menuOptions.emplace_back("spacer", "");
    }

    int PollEvent(sf::Event event) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return 0;
        }

        if (/*event.type == sf::Event::MouseButtonPressed */ sf::Mouse::isButtonPressed(sf::Mouse::Left)&&
            event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (mousePos.x < 0 || mousePos.x >= width ||
                    mousePos.y < 0 || mousePos.y >= height) {
                    window.close();
                    return 1;
                }
                
                int y = event.mouseButton.y;
                int clickedIndex = y / SPACING;
                if (clickedIndex >= 0 && clickedIndex < menuOptions.size()) {
                    const auto& [text, command] = menuOptions[clickedIndex];
                    if (!command.empty() && text != "spacer") {
                        std::system((command + " &").c_str());
                        window.close();
                        return 1;
                    }
                }
            }
         return 0;
    }

    void renderMenuOption(int i, sf::Font& font) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        const auto& [text, command] = menuOptions[i];
        int itemY = i * SPACING;
        
        if (text == "spacer") {
            string str(30, '_');
            sf::Text spacer = createText(str.c_str(), font, FONT_SIZE - 4, 10, itemY + SPACING / 8);
            spacer.setFillColor(sf::Color(128, 128, 128));
            window.draw(spacer);
        } else {
            if(mousePos.x >= 0 && mousePos.x <= width &&
               mousePos.y >= itemY &&
               mousePos.y <= itemY + SPACING) {
                sf::RectangleShape highlightRect = createHighlightRect(8, itemY, width - 16, SPACING - 4);
                window.draw(highlightRect);
            }
            sf::Text menuText = createText(text, font, FONT_SIZE, 10, itemY + 4);
            window.draw(menuText);
        }
    }
    
    int run() {
        sf::Font font;
        if (!font.loadFromFile("./SF-Pro.ttf"))
            return 1;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event))
                if(PollEvent(event)) break;

            window.clear(backgroundColor);

            for (size_t i = 0; i < menuOptions.size(); ++i)
                renderMenuOption(i, font);

            window.display();
        }
        return 0;
    }
};

int apple() {
    std::array<int, 4> dimensions = {3, 30, 200, 285};
    DropdownPanel panel(dimensions, sf::Color(241, 239, 234));

    panel.addMenuOption("About This Computer", "echo 'About This Computer'");
    panel.addSpacer();
    panel.addMenuOption("System Settings...", "echo 'System Settings...'");
    panel.addMenuOption("App Store...", "echo 'App Store...'");
    panel.addSpacer();
    panel.addMenuOption("Recent Items", "echo 'Recent Items'");
    panel.addSpacer();
    panel.addMenuOption("Force Quit Apps", "echo 'Force Quit Apps'");
    panel.addSpacer();
    panel.addMenuOption("Sleep", "echo 'Sleep'");
    panel.addMenuOption("Restart...", "echo 'Restart...'");
    panel.addMenuOption("Shut Down...", "echo 'Shut Down...'");
    panel.addSpacer();
    panel.addMenuOption("Lock Screen", "echo 'Lock Screen'");

    return panel.run();
}

int browser() {
    std::array<int, 4> dimensions = {40, 30, 200, 285};
    DropdownPanel panel(dimensions, sf::Color(241, 239, 234));

    panel.addMenuOption("Open Tab", "echo 'Open Tab'");
    panel.addSpacer();
    panel.addMenuOption("New Window", "echo 'New Window'");
    panel.addMenuOption("History", "echo 'History'");
    panel.addSpacer();
    panel.addMenuOption("Bookmarks", "echo 'Bookmarks'");
    panel.addSpacer();
    panel.addMenuOption("Downloads", "echo 'Downloads'");
    panel.addSpacer();
    panel.addMenuOption("Settings", "echo 'Settings'");
    panel.addMenuOption("Extensions", "echo 'Extensions'");
    panel.addSpacer();
    panel.addMenuOption("Help", "echo 'Help'");
    
    return panel.run();
}

int main(int argc, char* argv[]) {
    string flag(argv[1]);
    if (flag == "--apple") {
        return apple();
    } else if (flag == "--browser") {
        return browser();
    } else {
        return 1;
    }
}

