#pragma once
#include "Engines/EntityComponents/Mutable.h"
#include "Graphics/OpenGL/Primitives/Primitive.h"
#include "OpenGL/Renderer/RendererGL.h"
#include <vector>
#include <memory>

class Command
{
public:
    inline virtual void execute() = 0;
    inline virtual void undo() = 0;//consider making access to this protected (only for use through CommandLog as a Friend class)

protected:
    // We do no want instances of Command.
    inline Command() = default;

private:
    // Children will have data here required for the command.

};

class TranslateCommand:public Command {
private:
    glm::vec3 translation;
    Translatable* target;
public:
    inline TranslateCommand(const glm::vec2& translation, Translatable* target) : translation({ translation, 0.f }), target(target) {};
    inline TranslateCommand(const glm::vec3& translation, Translatable* target) : translation(translation), target(target) {};
    void execute() {
        if (target)
            target->translate(translation);
    }
    void undo() {
        target->translate(-translation);
    }
};

class Translate2DCommand :public Command {
private:
    glm::vec2 translation;
    Translatable2D* target;
public:
    inline Translate2DCommand(const glm::vec2& translation, Translatable2D* target) : translation(translation), target(target) {};
    void execute() {
        if (target)
            target->translate(translation);
    }
    void undo() {
        target->translate(-translation);
    }
};

class SetColourCommand:public Command {
private:
    glm::vec4 old_colour;
    glm::vec4 new_colour;
    IPrimitive* target;
public:
    inline SetColourCommand(const glm::vec4& new_colour, IPrimitive* target):new_colour(new_colour),old_colour(target->m_colour), target(target) {};
    void execute() {
        if (target)
            target->setColor(new_colour);
    }
    void undo() {
        target->setColor(old_colour);
    }
};

class RemoveCommand:public Command {
private:
    Removable* target;
public:
    inline RemoveCommand(Removable* target):target(target) {}
    void execute() {
        if (target)
            target->remove();
    }
    void undo() {
        target->restore();
    }
};

class RemovePrimitveCommand:public Command {
private:
    IPrimitive* target;
public:
    inline RemovePrimitveCommand(IPrimitive* target):target(target) {}
    void execute() {
        if (target)
            Renderer::remove(target);
    }
    void undo() {
        //add primitive back to renderer.
    }
};


class CommandLog
{
private:
    // Probably want to put a max size on this in future.
    std::vector<std::unique_ptr<Command>> m_commandHistory;
    unsigned next_command_ixd = 0;

public:
    void undo()
    {
        if (next_command_ixd == 0) return;

        next_command_ixd--;
        m_commandHistory.at(next_command_ixd)->undo();
        
    };

    void redo()
    {
        if (next_command_ixd == m_commandHistory.size()) return;

        m_commandHistory.at(next_command_ixd)->execute();
        next_command_ixd++;

    };
    template <class CommandType, class ... Args>
    void log(const Args&... args)
    {   
        if (next_command_ixd != m_commandHistory.size()) {
            //subsequent entries become invalid at this point
            m_commandHistory.resize(next_command_ixd);
        }
        m_commandHistory.emplace_back(std::make_unique<CommandType>(args...));
        next_command_ixd++;
    };

    template <class CommandType, class ... Args>
    void execute(const Args&... args)
    {
        log<CommandType>(args...);
        m_commandHistory[next_command_ixd-1]->execute();
    }

    const auto& getHistory()
    {
        return m_commandHistory;
    }
};
