#include "Command.h"
#include "Engines/EngineCore/EngineCore.h"

void CommandLog::undo()
{
    if (next_command_ixd == 0) return;

    next_command_ixd--;
    m_commandHistory.at(next_command_ixd)->undo();

    switch (m_commandHistory.at(next_command_ixd)->commandType) {
    case CommandType::CommandType_Remove:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Undo delete");
        break;
    case CommandType::CommandType_RemovePrimitive:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Undo delete");
        break;
    case CommandType::CommandType_SetColour:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Undo colour change");
        break;
    case CommandType::CommandType_Translate:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Undo translation");
        break;
    case CommandType::CommandType_Translate2D:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Undo translation");
        break;
    case CommandType::CommandType_ChangeValue:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Undo edit");
        break;
    case CommandType::CommandType_TranslateVertex:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Undo edit");
        break;
    default:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Undo action");
        break;
    }
    notifyParent();
};

void CommandLog::redo()
{
    if (next_command_ixd == m_commandHistory.size()) return;

    m_commandHistory.at(next_command_ixd)->execute();

    switch (m_commandHistory.at(next_command_ixd)->commandType) {
    case CommandType::CommandType_Remove:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Redo delete");
        break;
    case CommandType::CommandType_RemovePrimitive:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Redo delete");
        break;
    case CommandType::CommandType_SetColour:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Redo colour change");
        break;
    case CommandType::CommandType_Translate:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Redo translation");
        break;
    case CommandType::CommandType_Translate2D:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Redo translation");
        break;
    case CommandType::CommandType_ChangeValue:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Redo edit");
        break;
    default:
        Lumen::getApp().pushNotification(NotificationType::Success, 500, "Redo action");
        break;
    }
    next_command_ixd++;
    notifyParent();
}
void CommandLog::notifyParent()
{
    parent->unsavedDocument();
}
;

