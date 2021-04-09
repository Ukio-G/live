#include "imgui.h"
#include "imgui-SFML.h"
#include <GL/glew.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cstdlib>
#include <thread>
#include "include/World.h"
#include "include/Basics.h"
#include "include/Shader.h"
#include "include/Executing.h"
#include <ctime>
World w;

void buttonsHandle(Executor & executor);
void disassenblyMenu(Executor & executor);

Executable * selectExecutable(sf::RenderWindow &window, World &world);

void updateColorBuffer(GLuint buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(w.colors), w.colors, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


static bool global_running = true;
static Executable * selected_executable = nullptr;

int main()
{
    std::srand(std::time(nullptr));
    sf::ContextSettings settings;

    Executor executor = {&w};
    ExecutingPool *ePool = new ExecutingPool();
    auto *energyPool = new WorldObjectsPool<EnergySource>();
    ePool->init();
    energyPool->init();
    w.ePoolPtr = ePool;
    w.energyPool = energyPool;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);
    ImGui::SFML::Init(window);
    // activate the window
    window.setActive(true);
    sf::Clock deltaClock;

    glewInit();
    ShaderProgram shader(new Shader("../shaders/vs.glsl", GL_VERTEX_SHADER),new Shader("../shaders/fs.glsl", GL_FRAGMENT_SHADER));

    unsigned int VBO, COLOR, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &COLOR);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(w.vertices), w.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, COLOR);
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(w.colors), w.colors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // run the main loop
    bool running = true;
    w.appendExecutableRandomPosition(emptyAbilities, Settings::baseExecutableCount);
    iPoint point = {2,2};
    w.appendWorldObject<EnergySource>(point);
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                running = false;
            else if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                selected_executable = selectExecutable(window, w);
                selected_executable->color = {1.0,1.0,0.0};
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        disassenblyMenu(executor);
        buttonsHandle(executor);

        window.clear();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        updateColorBuffer(COLOR);

        shader.use();
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glPointSize(Settings::pointSize);
        glDrawArrays(GL_POINTS, 0, Settings::height * Settings::width);
        glBindVertexArray(0);

        ImGui::SFML::Render(window);
        window.display();
        if(global_running) executor.executeWorld();
    }
    ImGui::SFML::Shutdown();

    return 0;
}

Executable * selectExecutable(sf::RenderWindow &window, World &world) {
    float min_distance = 1.0f;
    int x_res, y_res;
    fPoint clickpoint = screenToNDC({(float)sf::Mouse::getPosition(window).x,(float)sf::Mouse::getPosition(window).y},
                                    {(int)window.getSize().x,(int)window.getSize().y});

    for(int x = 0; x < Settings::width; x++){
        for(int y = 0; y < Settings::height; y++){
            if (w.executables[x][y] == nullptr) continue;
            auto position = w.vertices[pointToNum({x,y})];
            auto new_distance = fDistance(clickpoint, position);
            if ( new_distance < min_distance){
                min_distance = new_distance;
                x_res = x;
                y_res = y;
            }
        }
    }
    w.colors[pointToNum({x_res,y_res})] = {1.0,0.0,0.0};
    if(selected_executable != nullptr) selected_executable->color = (fColor)emptyAbilities;
    return w.executables[x_res][y_res];
}

void buttonsHandle(Executor & executor) {
    if (ImGui::Button("Pause")) {
        global_running = false;
    }

    if (ImGui::Button("Continue")) {
        global_running = true;
    }

    if (ImGui::Button("Make one step")) {
        if(!global_running)
            executor.executeWorld();
    }
}

void disassenblyMenu(Executor & executor)
{
    if(selected_executable != nullptr) {
        ImGui::Begin("Disassembly");

        for (int i = 0; i < Settings::programSize; i++) {
            Command_u & instruction = selected_executable->program[i];
            if (selected_executable->instructionPointer == i)
                ImGui::Text("* %02X %02X %02X %02X", instruction.command, instruction.argument[0], instruction.argument[1], instruction.argument[2]);
            else
                ImGui::Text("%02X %02X %02X %02X", instruction.command, instruction.argument[0], instruction.argument[1], instruction.argument[2]);
        }
        if (ImGui::Button("ExecuteCurrent")){
            executor.execute(selected_executable);
        }

        if (ImGui::Button("Next instruction")){
            selected_executable->instructionPointer++;
        }

        if (ImGui::Button("Previous instruction")){
            selected_executable->instructionPointer--;
        }

        if (ImGui::Button("Kill")){
            selected_executable->energy = 1;
        }

        ImGui::Text("Position %i %i", selected_executable->position.x,selected_executable->position.y );
        ImGui::Text("Energy %i", selected_executable->energy);
        ImGui::End();
    }
}