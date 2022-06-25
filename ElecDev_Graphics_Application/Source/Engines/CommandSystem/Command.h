#pragma once
#include "Engines/EntityComponents/Mutable.h"
#include "Graphics/OpenGL/Primitives/Primitive.h"
#include "Graphics/OpenGL/Primitives/Text.h"
#include "OpenGL/Renderer/RendererGL.h"
#include <vector>
#include <memory>
#include "Utilities/Logger/Logger.h"
#include "Application/Application.h"
#include "Lumen/Lumen.h"
#include <functional>

//typedef uint64_t LumenCommandID;
enum class CommandType
{
    CommandType_None,//This should never be seen in practice
    CommandType_Translate,
    CommandType_Translate2D,
    CommandType_SetColour,
    CommandType_Remove,
    CommandType_RemovePrimitive,
    CommandType_ChangeValue,
    CommandType_TranslateVertex
};



class Command
{
public:
    CommandType commandType = CommandType::CommandType_None;
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
    const glm::vec3 translation;
    Translatable* const target;
public:
    inline TranslateCommand(const glm::vec2& translation, Translatable* target) : translation({ translation, 0.f }), target(target) { commandType = CommandType::CommandType_Translate; };
    inline TranslateCommand(const glm::vec3& translation, Translatable* target) : translation(translation), target(target) { commandType = CommandType::CommandType_Translate; };
    void execute() {
        try {
            target->translate(translation);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to execute a translation on an invlaid target.", "Command");
        }
    }
    void undo() {
        try {
            target->translate(-translation);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to undo a translation on an invlaid target.", "Command");
        }
    }
};

class Translate2DCommand :public Command {
private:
    const glm::vec2 translation;
    Translatable2D* const target;
public:
    inline Translate2DCommand(const glm::vec2& translation, Translatable2D* target) : translation(translation), target(target) { commandType = CommandType::CommandType_Translate2D; };
    void execute() {
        try {
            target->translate(translation);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to execute a translation on an invlaid target.", "Command");
        }
    }
    void undo() {
        try {
            target->translate(-translation);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to undo a translation on an invlaid target.", "Command");
        }
    }
};


class TranslateVertexCommand :public Command {
private:
    const glm::vec2 translation;
    const unsigned index;
    Reshapable* const target;
public:
    inline TranslateVertexCommand(const unsigned& localIndex, const glm::vec2& translation, Reshapable* target) : index(localIndex), translation(translation), target(target) { commandType = CommandType::CommandType_TranslateVertex; };
    void execute() {
        try {
            target->translateVertexAtIndex(index, translation);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to execute a vertex translation on an invlaid target.", "Command");
        }
    }
    void undo() {
        try {
            target->translateVertexAtIndex(index, -translation);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to undo a vertex translation on an invlaid target.", "Command");
        }
    }
};

class SetColourCommand:public Command {
private:
    const glm::vec4 old_colour;
    const glm::vec4 new_colour;
    IPrimitive* const target;
public:
    inline SetColourCommand(const glm::vec4& new_colour, IPrimitive* target) :new_colour(new_colour), old_colour(target->m_colour), target(target) { commandType = CommandType::CommandType_SetColour; };
    inline SetColourCommand(const glm::vec4& new_colour, IPrimitive* target, const glm::vec4& old_colour) :new_colour(new_colour), old_colour(old_colour), target(target) { commandType = CommandType::CommandType_SetColour; };
    void execute() {
        try {
            target->setColor(new_colour);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to execute a colour change on an invlaid target.", "Command");
        }
        
    }
    void undo() {
        try {
            target->setColor(old_colour);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to undo a colour change on an invlaid target.", "Command");
        }
    }
};

class RemoveCommand:public Command {
private:
    Removable* const target;
public:
    inline RemoveCommand(Removable* target):target(target) { commandType = CommandType::CommandType_Remove; }
    void execute() {
        try {
            target->remove();
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to remove an invlaid target.", "Command");
        }
    }
    void undo() {
        try {
            target->restore();
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to restore an invlaid target.", "Command");
        }
    }
};

class EditTextCommand :public Command {
    const std::string oldString;
    const std::string newString;
    Text* const target;
public:
    inline EditTextCommand(Text* target, const std::string& newString, const std::string& oldString) :target(target), oldString(oldString), newString(newString) { commandType = CommandType::CommandType_ChangeValue; };
    inline EditTextCommand(Text* target, const std::string& newString) : EditTextCommand(target, newString, target->m_string) {};
    void execute() {
        target->updateText(newString);
    }
    void undo() {
        target->updateText(oldString);
    }
};

class RemovePrimitveCommand:public Command {
private:
    IPrimitive* const target;
public:
    inline RemovePrimitveCommand(IPrimitive* target):target(target) { commandType = CommandType::CommandType_RemovePrimitive; }
    void execute() {
        try {
            Renderer::remove(target);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to remove an invlaid primitive.", "Command");
        }
    }
    void undo() {
        //add primitive back to renderer.
        try {

        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to remove an invlaid primitive.", "Command");
        }
    }
};

template <typename T>
class ChangeValueCommand: public Command{
private:
    const T oldVal;
    const T newVal;
    T* const target;
public:
    inline ChangeValueCommand(const T& newValue, T* target) : oldVal(*target), newVal(newValue), target(target) { commandType = CommandType::CommandType_ChangeValue; };
    template <typename T>
    inline ChangeValueCommand(const T& newValue, T* target, const T& oldValue) : oldVal(oldValue), newVal(newValue), target(target) { commandType = CommandType::CommandType_ChangeValue; };
    inline void execute() {
        try {
            *target = newVal;
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to change the value of an invlaid target.", "Command");
        }
    }
    inline void undo() {
        try {
            *target = oldVal;
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to change the value of an invlaid target.", "Command");
        }
    }
};

template <typename T, class C>
class ChangeValueWithSetterCommand : public Command {
private:
    const T oldVal;
    const T newVal;
    C* const target;

    std::function<void(C*, T)> setter;

public:
    inline ChangeValueWithSetterCommand(const T& newValue, C* target, const T& oldValue, std::function<void(C*, T)> setter) :
        oldVal(oldValue), newVal(newValue), target(target), setter(setter) { commandType = CommandType::CommandType_ChangeValue; };
    inline void execute() {
        try {
            setter(target, newVal);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to change the value of an invlaid target.", "Command");
        }
    }
    inline void undo() {
        try {
            setter(target, oldVal);
        }
        catch (...) {
            LUMEN_LOG_WARN("Tried to change the value of an invlaid target.", "Command");
        }
    }
};



class EngineCore;
class CommandLog
{
private:
    // Probably want to put a max size on this in future.
    std::vector<std::unique_ptr<Command>> m_commandHistory;
    unsigned next_command_ixd = 0;
    EngineCore* parent;//Primarily used to notify the engine when a command is logged

    void notifyParent();

public:
    inline CommandLog(EngineCore* parent) :parent(parent) {};
    void undo();
    void redo();
    template <typename cType, class ... Args>
    void log(const Args&... args)
    {   
        if (next_command_ixd != m_commandHistory.size()) {
            //subsequent entries become invalid at this point
            m_commandHistory.resize(next_command_ixd);
        }
        m_commandHistory.emplace_back(std::make_unique<cType>(args...));
        next_command_ixd++;

        //Notify engine of logged change
        notifyParent();
    };

    template <class cType, class ... Args>
    void execute(const Args&... args)
    {
        log<cType>(args...);
        m_commandHistory[next_command_ixd - 1]->execute();
    };

    const auto& getHistory()
    {
        return m_commandHistory;
    };
};
