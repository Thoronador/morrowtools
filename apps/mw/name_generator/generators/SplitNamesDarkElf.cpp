/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2023  Dirk Stolle

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

#include "SplitNamesDarkElf.hpp"
#include <random>
#include "../../../../lib/mw/NPCs.hpp"

namespace MWTP
{

SplitNamesDarkElf::SplitNamesDarkElf()
: ashlander_first_names({ }),
  ashlander_last_names({ }),
  house_first_names({ }),
  house_last_names({ })
{
}

bool SplitNamesDarkElf::is_ashlander(const NPCRecord& npc)
{
  static const auto ashlanders_without_faction = {
    "abassel asserbassalit",
    "abibaal assillariran",
    "abishpulu shand",
    "adairan lalansour",
    "adaishah ahanidiran",
    "addarnat assardidairan",
    "adibael hainnabibi",
    "adusamsi assurnarairan",
    "ahanabi assullinbanud",
    "ainab yassabisun",
    "ainat maessabibi",
    "anasour selitbael",
    "anit eramarellaku",
    "asha-ammu kutebani",
    "ashamanu",
    "asharapli almu",
    "ashuma-nud matluberib",
    "ashumanu eraishah",
    "assamanut sonnerralit",
    "assatlit assudnilamat",
    "assirari zama-rasour",
    "assumanu mantiti",
    "assurdan serdimapal",
    "dakin kuntarnammu",
    "dun-ilu assurnumausur",
    "dunsalipal dun-ahhe",
    "elibael puntumisun",
    "esib-nummu assunudadnud",
    "ghost_npc_erur-dan",
    "hainab lasamsi",
    "hanarai assutlanipal",
    "hannabi zabynatus",
    "hannat zainsubani",
    "hassour zainsubani",
    "hemus zelma-alit",
    "hentus yansurnummu",
    "hibaddar ilath-pal",
    "hinammu shishara",
    "hinummu siddurnanit",
    "ibarnadad assirnarari",
    "ibasour sershurrapal",
    "ilasour tansumiran",
    "kanat assalatammis",
    "kanit niladon",
    "kanud odimabesser",
    "kitbael assurnumanit",
    "kummi-namus almu",
    "kund assarnibani",
    "maeonius man-llu",
    "maeonius yentimilal",
    "maesat shinirbael",
    "mal mibishanit",
    "malay ranarbatus",
    "manat shimmabadas",
    "manat varnan-adda",
    "massour asserrumusa",
    "mausur ababael",
    "mausur assaplit",
    "mibdinahaz addunipu",
    "minisun ulirbabi",
    "minnibi selkin-adda",
    "mubdan assantinalit",
    "mubdan esurnadarpal",
    "musa yahaz",
    "musan zaintashara",
    "mut urshan-adairan",
    "nammu man-llu",
    "nind dudnebisun",
    "ninimilk addinibi",
    "nirait shin-ilu",
    "nund assurnipilu",
    "odairan addaribantes",
    "odairan ashummi-ammus",
    "odaishah yasalmibaal",
    "ohibaal assintashiran",
    "patus assumanallit",
    "pilun ashapaladdon",
    "punibi yahaz",
    "rasamsi esunarnat",
    "sal pudashara",
    "salay yessur-disadon",
    "salmat elarrapal",
    "salmus kaushmamanu",
    "sargon santumatus",
    "selk muddumummu",
    "selkirnemus",
    "shanat kaushminipu",
    "shanit ashar-don",
    "shannat pansamsi",
    "shanud ududnabia",
    "shargon odin-ahhe",
    "shilipuran zama-rasour",
    "shin assemmus",
    "shipal zansatanit",
    "shirerib ashi-iddan",
    "shishi yanumibaal",
    "shulki ashunbabi",
    "shullay malman-ammu",
    "shullay vabdas",
    "subenend urshummarnamus",
    "sur surishpi",
    "talkin ashar-dad",
    "tashpi ashibael",
    "teshmus assebiriddan",
    "tibdan shalarnetus",
    "tis abalkala",
    "tissamsi asharapli",
    "turan tibashipal",
    "ulisamsi saddarnuran",
    "yahaz ashurnasaddas",
    "yakum hairshashishi",
    "yan assinabi",
    "yanit sehabani",
    "yapal esatliballit",
    "yassour hairshumusa",
    "zabarbael",
    "zainat ahalkalun",
    "zairan enturnabaelul",
    "zalabelk saladnius",
    "zalit salkatanat",
    "zallit assattadaishah",
    "zanat assarnuridan",
    "zanmulk sammalamus",
    "zebba benamamat",
    "zebdusipal mantiti",
    "zelaku assarrapanat",
    "zelay sobbinisun",
    "zenabi samma",
    "zennammu urshumusa"
  };

  return (npc.FactionID == "Ashlanders") || (npc.ClassID == "Mabrigash")
    || (npc.ClassID == "Wise Woman")
    || (std::find(ashlanders_without_faction.begin(),
                  ashlanders_without_faction.end(), npc.recordID)
        != ashlanders_without_faction.end());
}

std::vector<std::string> SplitNamesDarkElf::generate_ashlanders(const uint_least16_t n, const bool female)
{
  if (ashlander_first_names.empty() && ashlander_last_names.empty())
  {
    prepare(female);
  }

  return generate(n, ashlander_first_names, ashlander_last_names);
}

std::vector<std::string> SplitNamesDarkElf::generate_non_ashlanders(const uint_least16_t n, const bool female)
{
  if (house_first_names.empty() && house_last_names.empty())
  {
    prepare(female);
  }

  return generate(n, house_first_names, house_last_names);
}

std::vector<std::string> SplitNamesDarkElf::generate(const uint_least16_t n,
    const std::vector<std::string>& first_names,
    const std::vector<std::string>& last_names)
{
  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<std::vector<std::string>::size_type> first_distrib(0, first_names.size() - 1);
  std::uniform_int_distribution<std::vector<std::string>::size_type> last_distrib(0, last_names.size() - 1);

  std::vector<std::string> result;
  for (uint_least16_t i = 0; i < n; ++i)
  {
    result.emplace_back(first_names[first_distrib(generator)] + " " + last_names[last_distrib(generator)]);
  }

  return result;
}

void SplitNamesDarkElf::prepare(const bool female)
{
  auto iter = NPCs::get().begin();
  while (iter != NPCs::get().end())
  {
    if (iter->second.RaceID == "Dark Elf")
    {
      const auto pos = iter->second.Name.find(' ');
      const bool is_female = iter->second.isFemale();
      const bool is_ashlander = SplitNamesDarkElf::is_ashlander(iter->second);
      if (pos == std::string::npos)
      {
        // First name only - can only be used if gender matches.
        if (female == is_female)
        {
          if (is_ashlander)
            ashlander_first_names.push_back(iter->second.Name);
          else
            house_first_names.push_back(iter->second.Name);
        }
      }
      else
      {
        if (female == is_female)
        {
          if (is_ashlander)
          {
            ashlander_first_names.push_back(iter->second.Name.substr(0, pos));
            ashlander_last_names.push_back(iter->second.Name.substr(pos + 1));
          }
          else
          {
            house_first_names.push_back(iter->second.Name.substr(0, pos));
            house_last_names.push_back(iter->second.Name.substr(pos + 1));
          }
        }
        else
        {
          // Last name is gender-neutral, so it can be used, too.
          if (is_ashlander)
            ashlander_last_names.push_back(iter->second.Name.substr(pos + 1));
          else
            house_last_names.push_back(iter->second.Name.substr(pos + 1));
        }
      }
    }
    ++iter;
  }
}

} // namespace
