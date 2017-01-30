#include <regex>

#include <apps/openmw-mp/Player.hpp>
#include <apps/openmw-mp/Script/ScriptFunctions.hpp>
#include <apps/openmw-mp/Networking.hpp>
#include <components/openmw-mp/NetworkMessages.hpp>
#include <components/openmw-mp/Base/WorldEvent.hpp>
#include "World.hpp"

using namespace mwmp;

static WorldEvent *worldEvent = nullptr;

std::regex exteriorCellPattern("^(-?\\d+), (-?\\d+)$");

void WorldFunctions::CreateWorldEvent(unsigned short pid) noexcept
{
    Player *player;
    GET_PLAYER(pid, player, );

    if (worldEvent)
    {
        delete worldEvent;
        worldEvent = nullptr;
    }

    worldEvent = new WorldEvent(player->guid);
}

void WorldFunctions::AddWorldObject() noexcept
{
    WorldObject worldObject;

    worldEvent->objectChanges.objects.push_back(worldObject);
}

void WorldFunctions::SetWorldEventCell(const char* cellDescription) noexcept
{
    std::string description = cellDescription;
    std::smatch baseMatch;

    if (std::regex_match(description, baseMatch, exteriorCellPattern))
    {
        worldEvent->cell.mData.mFlags &= ~ESM::Cell::Interior;

        // The first sub match is the whole string, so check for a length of 3
        if (baseMatch.size() == 3) {

            worldEvent->cell.mData.mX = stoi(baseMatch[1].str());
            worldEvent->cell.mData.mY = stoi(baseMatch[2].str());
        }
    }
    else
    {
        worldEvent->cell.mData.mFlags |= ESM::Cell::Interior;
        worldEvent->cell.mName = description;
    }
}

void WorldFunctions::SetObjectRefId(unsigned int i, const char* refId) noexcept
{
    worldEvent->objectChanges.objects[i].refId = std::string(refId);
}

void WorldFunctions::SetObjectRefNumIndex(unsigned int i, int refNumIndex) noexcept
{
    worldEvent->objectChanges.objects[i].refNumIndex = refNumIndex;
}

void WorldFunctions::SetObjectCount(unsigned int i, int count) noexcept
{
    worldEvent->objectChanges.objects[i].count = count;
}

void WorldFunctions::SetObjectGoldValue(unsigned int i, int goldValue) noexcept
{
    worldEvent->objectChanges.objects[i].goldValue = goldValue;
}

void WorldFunctions::SetObjectPosition(unsigned int i, double x, double y, double z) noexcept
{
    worldEvent->objectChanges.objects[i].pos.pos[0] = x;
    worldEvent->objectChanges.objects[i].pos.pos[1] = y;
    worldEvent->objectChanges.objects[i].pos.pos[2] = z;
}

void WorldFunctions::SetObjectRotation(unsigned int i, double x, double y, double z) noexcept
{
    worldEvent->objectChanges.objects[i].pos.rot[0] = x;
    worldEvent->objectChanges.objects[i].pos.rot[1] = y;
    worldEvent->objectChanges.objects[i].pos.rot[2] = z;
}

unsigned int WorldFunctions::GetObjectChangesSize() noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.count;
}

const char *WorldFunctions::GetObjectRefId(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].refId.c_str();
}

int WorldFunctions::GetObjectRefNumIndex(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].refNumIndex;
}

int WorldFunctions::GetObjectCount(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].count;
}

int WorldFunctions::GetObjectGoldValue(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].goldValue;
}

double WorldFunctions::GetObjectPosX(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].pos.pos[0];
}

double WorldFunctions::GetObjectPosY(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].pos.pos[1];
}

double WorldFunctions::GetObjectPosZ(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].pos.pos[2];
}

double WorldFunctions::GetObjectRotX(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].pos.rot[0];
}

double WorldFunctions::GetObjectRotY(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].pos.rot[1];
}

double WorldFunctions::GetObjectRotZ(unsigned int i) noexcept
{
    return mwmp::Networking::getPtr()->getLastEvent()->objectChanges.objects[i].pos.rot[2];
}

void WorldFunctions::SendObjectDelete() noexcept
{
    mwmp::Networking::get().getWorldController()->GetPacket(ID_OBJECT_DELETE)->Send(worldEvent, worldEvent->guid);
}

void WorldFunctions::SendObjectPlace() noexcept
{
    mwmp::Networking::get().getWorldController()->GetPacket(ID_OBJECT_PLACE)->Send(worldEvent, worldEvent->guid);
}

void WorldFunctions::SetHour(unsigned short pid, double hour) noexcept
{
    Player *player;
    GET_PLAYER(pid, player,);

    player->hour = hour;
    player->month = -1;
    player->day = -1;

    mwmp::Networking::get().getPlayerController()->GetPacket(ID_GAME_TIME)->Send(player, false);
}

void WorldFunctions::SetMonth(unsigned short pid, int month) noexcept
{
    Player *player;
    GET_PLAYER(pid, player,);

    player->hour = -1;
    player->month = month;
    player->day = -1;

    mwmp::Networking::get().getPlayerController()->GetPacket(ID_GAME_TIME)->Send(player, false);

}

void WorldFunctions::SetDay(unsigned short pid, int day) noexcept
{
    Player *player;
    GET_PLAYER(pid, player,);

    player->hour = -1;
    player->month = -1;
    player->day = day;

    mwmp::Networking::get().getPlayerController()->GetPacket(ID_GAME_TIME)->Send(player, false);
}
