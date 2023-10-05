/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022, 2023  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "PreNPCRecord.hpp"
#include "../MW_Constants.hpp"

namespace MWTP
{

const int32_t pnfTraining = 16384;
const int32_t pnfSpellmaking = 32768;
const int32_t pnfEnchanting = 65536;
const int32_t pnfRepair = 131072;

PreNPCRecord::PreNPCRecord()
: BasicRecord(),
  Items(std::vector<ItemRecord>()),
  NPC_Spells(std::vector<std::string>()),
  AIData(std::nullopt),
  AIPackages(std::vector<NPC_BasicAIPackage*>()),
  Destinations(std::vector<TravelDestination>())
{
}

PreNPCRecord::~PreNPCRecord()
{
  removeAIPackages();
}

bool PreNPCRecord::hasEqualAIPackages(const PreNPCRecord& other) const
{
  const auto len = AIPackages.size();
  if (other.AIPackages.size() != len)
    return false;
  for (decltype(AIPackages)::size_type i = 0; i < len; ++i)
  {
    if ((AIPackages[i] == nullptr) ^ (other.AIPackages[i] == nullptr))
      return false;
    if (AIPackages[i] != nullptr)
    {
      // Do they have the same type?
      if (AIPackages[i]->getPackageType() != other.AIPackages[i]->getPackageType())
      {
        return false;
      }
      switch (AIPackages[i]->getPackageType())
      {
        case PackageType::ptActivate:
             if (!(static_cast<NPC_AIActivate*>(AIPackages[i]))->equals(*static_cast<NPC_AIActivate*>(other.AIPackages[i])))
               return false;
             break;
        case PackageType::ptEscort:
        case PackageType::ptFollow:
             if (!(static_cast<NPC_AIEscortFollow*>(AIPackages[i]))->equals(*static_cast<NPC_AIEscortFollow*>(other.AIPackages[i])))
               return false;
             break;
        case PackageType::ptTravel:
             if (!(static_cast<NPC_AITravel*>(AIPackages[i]))->equals(*static_cast<NPC_AITravel*>(other.AIPackages[i])))
               return false;
             break;
        case PackageType::ptWander:
             if (!(static_cast<NPC_AIWander*>(AIPackages[i]))->equals(*static_cast<NPC_AIWander*>(other.AIPackages[i])))
               return false;
             break;
      }
    }
  }
  return true;
}

void PreNPCRecord::removeAIPackages()
{
  while (!AIPackages.empty())
  {
    delete AIPackages.back();
    AIPackages.pop_back();
  }
}

void PreNPCRecord::copyAIPackages(const PreNPCRecord& source)
{
  for (decltype(source.AIPackages)::size_type i = 0; i < source.AIPackages.size(); ++i)
  {
    if (source.AIPackages[i] == nullptr)
    {
      AIPackages.push_back(nullptr);
      continue;
    }
    switch (source.AIPackages[i]->getPackageType())
    {
      case PackageType::ptActivate:
           {
             NPC_AIActivate* pkgPtr = new NPC_AIActivate;
             *pkgPtr = *(static_cast<NPC_AIActivate*>(source.AIPackages[i]));
             AIPackages.push_back(pkgPtr);
           }
           break;
      case PackageType::ptEscort:
           {
             NPC_AIEscort* pkgPtr = new NPC_AIEscort;
             *pkgPtr = *(static_cast<NPC_AIEscort*>(source.AIPackages[i]));
             AIPackages.push_back(pkgPtr);
           }
           break;
      case PackageType::ptFollow:
           {
             NPC_AIFollow* pkgPtr = new NPC_AIFollow;
             *pkgPtr = *(static_cast<NPC_AIFollow*>(source.AIPackages[i]));
             AIPackages.push_back(pkgPtr);
           }
           break;
      case PackageType::ptTravel:
           {
             NPC_AITravel* pkgPtr = new NPC_AITravel;
             *pkgPtr = *(static_cast<NPC_AITravel*>(source.AIPackages[i]));
             AIPackages.push_back(pkgPtr);
           }
           break;
      case PackageType::ptWander:
           {
             NPC_AIWander* pkgPtr = new NPC_AIWander;
             *pkgPtr = *(static_cast<NPC_AIWander*>(source.AIPackages[i]));
             AIPackages.push_back(pkgPtr);
           }
           break;
    }
  }
}

bool PreNPCRecord::isTrainer() const
{
  return AIData.has_value() && ((AIData.value().Flags & pnfTraining) != 0);
}

bool PreNPCRecord::isEnchanter() const
{
  return AIData.has_value() && ((AIData.value().Flags & pnfEnchanting) != 0);
}

bool PreNPCRecord::isSpellmaker() const
{
  return AIData.has_value() && ((AIData.value().Flags & pnfSpellmaking) != 0);
}

bool PreNPCRecord::doesRepair() const
{
  return AIData.has_value() && ((AIData.value().Flags & pnfRepair) != 0);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool PreNPCRecord::writeItemsSpellsAIDataDestinations(std::ostream& output) const
{
  // Items
  for (const auto& item: Items)
  {
    if (!item.saveToStream(output))
      return false;
  }

  // Spells
  for (const auto& spell: NPC_Spells)
  {
    // write spell ID (NPCS)
    output.write(reinterpret_cast<const char*>(&cNPCS), 4);
    const uint32_t SubLength = 32;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    // write spell ID
    std::string::size_type len = spell.length();
    if (len > 31)
    {
      len = 31;
    }
    output.write(spell.c_str(), len);
    // fill rest with NUL bytes
    output.write(NULof32, 32 - len);
  }

  if (AIData.has_value())
  {
    // write AI data (AIDT)
    if (!AIData.value().saveToStream(output))
      return false;
  }

  // travel service destinations
  for (const auto& destination: Destinations)
  {
    if (!destination.saveToStream(output))
      return false;
  }

  // AI packages
  for (const auto& pkg: AIPackages)
  {
    if (!pkg->saveToStream(output))
      return false;
  }

  return output.good();
}
#endif

} // namespace
