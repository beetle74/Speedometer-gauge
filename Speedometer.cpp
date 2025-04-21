#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

class Speedometer {
private:
    sf::RenderWindow& window;
    float radius; // Radius of the gauge
    sf::Vector2f center; // Center of the gauge
    float speed; // Current speed in MPH
    const float maxSpeed = 120.0f; // Maximum speed (MPH)
    sf::Font font; // Font for labels

public:
    Speedometer(sf::RenderWindow& win, float r, sf::Vector2f c)
        : window(win), radius(r), center(c), speed(0.0f) {
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Failed to load font file!");
        }
    }

    void setSpeed(float s) {
        speed = std::max(0.0f, std::min(maxSpeed, s));
    }

    void draw() {
        sf::CircleShape bezel(radius + 20);
        bezel.setFillColor(sf::Color(220, 220, 220));
        bezel.setPosition(center - sf::Vector2f(radius + 20, radius + 20));
        window.draw(bezel);

        sf::CircleShape gauge(radius);
        gauge.setFillColor(sf::Color(30, 30, 30));
        gauge.setPosition(center - sf::Vector2f(radius, radius));
        window.draw(gauge);

        for (int i = 0; i <= 120; i += 10) {
            float angle = (i / maxSpeed) * 270.0f - 135.0f;
            float rad = angle * (3.14159f / 180.0f);

            if (i % 20 == 0) {
                sf::Vertex tick[] = {
                    sf::Vertex(center + sf::Vector2f(radius * 0.8f * cos(rad), radius * 0.8f * sin(rad)), sf::Color::White),
                    sf::Vertex(center + sf::Vector2f(radius * 0.95f * cos(rad), radius * 0.95f * sin(rad)), sf::Color::White)
                };
                window.draw(tick, 2, sf::Lines);

                sf::Text label;
                label.setFont(font);
                label.setString(std::to_string(i));
                label.setCharacterSize(40);
                label.setFillColor(sf::Color::White);
                sf::FloatRect bounds = label.getLocalBounds();
                label.setOrigin(bounds.width / 2, bounds.height / 2);
                label.setPosition(center + sf::Vector2f(radius * 0.65f * cos(rad), radius * 0.65f * sin(rad)));
                window.draw(label);
            } else {
                sf::Vertex tick[] = {
                    sf::Vertex(center + sf::Vector2f(radius * 0.85f * cos(rad), radius * 0.85f * sin(rad)), sf::Color::White),
                    sf::Vertex(center + sf::Vector2f(radius * 0.95f * cos(rad), radius * 0.95f * sin(rad)), sf::Color::White)
                };
                window.draw(tick, 2, sf::Lines);
            }

            if (i == 60) {
                sf::Vertex highlight[] = {
                    sf::Vertex(center + sf::Vector2f(radius * 0.8f * cos(rad), radius * 0.8f * sin(rad)), sf::Color::Green),
                    sf::Vertex(center + sf::Vector2f(radius * 0.95f * cos(rad), radius * 0.95f * sin(rad)), sf::Color::Green)
                };
                window.draw(highlight, 2, sf::Lines);
            }
        }

        float needleAngle = (speed / maxSpeed) * 270.0f - 135.0f;
        float needleRad = needleAngle * (3.14159f / 180.0f);
        sf::Vertex needle[] = {
            sf::Vertex(center, sf::Color(255, 255, 255)),
            sf::Vertex(center + sf::Vector2f(radius * 0.8f * cos(needleRad), radius * 0.8f * sin(needleRad)), sf::Color(255, 255, 255))
        };
        window.draw(needle, 2, sf::Lines);

        sf::CircleShape centerCap(radius * 0.1f);
        centerCap.setFillColor(sf::Color(50, 50, 50));
        centerCap.setPosition(center - sf::Vector2f(radius * 0.1f, radius * 0.1f));
        window.draw(centerCap);

        sf::Text mphLabel;
        mphLabel.setFont(font);
        mphLabel.setString("MPH");
        mphLabel.setCharacterSize(30);
        mphLabel.setFillColor(sf::Color::White);
        sf::FloatRect mphBounds = mphLabel.getLocalBounds();
        mphLabel.setOrigin(mphBounds.width / 2, mphBounds.height / 2);
        mphLabel.setPosition(center + sf::Vector2f(0, radius * 0.5f));
        window.draw(mphLabel);

        sf::RectangleShape digitalDisplay(sf::Vector2f(120, 40));
        digitalDisplay.setFillColor(sf::Color(10, 10, 10));
        digitalDisplay.setOutlineColor(sf::Color::White);
        digitalDisplay.setOutlineThickness(2);
        digitalDisplay.setPosition(center + sf::Vector2f(-60, radius * 0.2f));
        window.draw(digitalDisplay);

        sf::Text digitalSpeed;
        digitalSpeed.setFont(font);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << speed;
        digitalSpeed.setString(ss.str());
        digitalSpeed.setCharacterSize(30);
        digitalSpeed.setFillColor(sf::Color::White);
        sf::FloatRect speedBounds = digitalSpeed.getLocalBounds();
        digitalSpeed.setOrigin(speedBounds.width / 2, speedBounds.height / 2);
        digitalSpeed.setPosition(center + sf::Vector2f(0, radius * 0.2f + 20));
        window.draw(digitalSpeed);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(720, 720), "Speedometer");
    window.setFramerateLimit(60);

    Speedometer speedometer(window, 300, sf::Vector2f(720 / 2, 720 / 2));

    float speed = 0.0f;
    bool increasing = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (increasing) {
            speed += 0.5f;
            if (speed >= 120.0f) increasing = false;
        } else {
            speed -= 0.5f;
            if (speed <= 0.0f) increasing = true;
        }
        speedometer.setSpeed(speed);

        window.clear(sf::Color(100, 100, 100));
        speedometer.draw();
        window.display();
    }

    return 0;
}
