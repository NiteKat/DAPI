#include "Item.h"

#include <sstream>

#include "Spell.h"

namespace DAPI
{
  Item::Item()
  {
    data = std::make_shared<ItemData>();
  }

  std::string Item::getName()
  {
    if (data->_iIdentified)
      return data->_iIName;
    else
      return data->_iName;
  }

  int Item::getValue()
  {
    return data->_ivalue;
  }

  ItemSpecialEffect Item::getFasterAttack()
  {
    if (data->_iFlags == -1)
      return ItemSpecialEffect::NONE;

    if (data->_iFlags & static_cast<int>(ItemSpecialEffect::QUICKATTACK))
      return ItemSpecialEffect::QUICKATTACK;
    else if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTATTACK))
      return ItemSpecialEffect::FASTATTACK;
    else if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTERATTACK))
      return ItemSpecialEffect::FASTERATTACK;
    else if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTESTATTACK))
      return ItemSpecialEffect::FASTESTATTACK;
    else
      return ItemSpecialEffect::NONE;
  }

  ItemSpecialEffect Item::getHitRecovery()
  {
    if (data->_iFlags == -1)
      return ItemSpecialEffect::NONE;

    if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTRECOVER))
      return ItemSpecialEffect::FASTRECOVER;
    else if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTERRECOVER))
      return ItemSpecialEffect::FASTERRECOVER;
    else if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTESTRECOVER))
      return ItemSpecialEffect::FASTESTRECOVER;
    else
      return ItemSpecialEffect::NONE;
  }

  std::string Item::getEffectText()
  {
    std::stringstream returnValue;
    auto getText = [&](ItemEffectID eid) {
      switch (eid) {
      case ItemEffectID::TOHIT:
      case ItemEffectID::TOHIT_CURSE:
        returnValue << "chance to hit : ";
        if (data->_iPLToHit > 0)
          returnValue << "+";
        returnValue << data->_iPLToHit << "%";
        break;
      case ItemEffectID::DAMP:
      case ItemEffectID::DAMP_CURSE:
        if (data->_iPLDam > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << data->_iPLDam << "% damage";
        break;
      case ItemEffectID::TOHIT_DAMP:
      case ItemEffectID::TOHIT_DAMP_CURSE:
        returnValue << "to hit: ";
        if (data->_iPLToHit > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << data->_iPLToHit << "%, ";
        if (data->_iPLDam > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << data->_iPLDam << "% damage";
        break;
      case ItemEffectID::ACP:
      case ItemEffectID::ACP_CURSE:
        if (data->_iPLAC > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << data->_iPLAC << "% armor";
        break;
      case ItemEffectID::SETAC:
        returnValue << "armor class: " << data->_iAC;
        break;
      case ItemEffectID::AC_CURSE:
        returnValue << "armor class: " << data->_iAC; //should this have a -?
        break;
      case ItemEffectID::FIRERES:
        returnValue << "Resist Fire : ";
        if (data->_iPLFR < 75)
        {
          if (data->_iPLFR > 0)
            returnValue << "+";
          else
            returnValue << "-";
          returnValue << data->_iPLFR << "%";
        }
        else
          returnValue << "75% MAX";
        break;
      case ItemEffectID::LIGHTRES:
        returnValue << "Resist Lightning : ";
        if (data->_iPLLR < 75)
        {
          if (data->_iPLLR > 0)
            returnValue << "+";
          else
            returnValue << "-";
          returnValue << data->_iPLLR << "%";
        }
        else
          returnValue << "75% MAX";
        break;
      case ItemEffectID::MAGICRES:
        returnValue << "Resist Magic : ";
        if (data->_iPLMR < 75)
        {
          if (data->_iPLMR > 0)
            returnValue << "+";
          else
            returnValue << "-";
          returnValue << data->_iPLMR << "%";
        }
        else
          returnValue << "75% MAX";
        break;
      case ItemEffectID::ALLRES:
        returnValue << "Resist All : ";
        if (data->_iPLFR < 75)
        {
          if (data->_iPLFR > 0)
            returnValue << "+";
          else
            returnValue << "-";
          returnValue << data->_iPLFR << "%";
        }
        else
          returnValue << "75% MAX";
        break;
      case ItemEffectID::SPLLVLADD:
        if (data->_iSplLvlAdd == 1)
          returnValue << "spells are increased 1 level";
        if (data->_iSplLvlAdd == 2)
          returnValue << "spells are increased 2 levels";
        if (data->_iSplLvlAdd < 1)
          returnValue << "spells are decreased 1 level";
        break;
      case ItemEffectID::CHARGES:
        returnValue << "Extra charges";
        break;
      case ItemEffectID::SPELL:
      {
        Spell spell{ static_cast<SpellID>(data->_iSpell), SpellTypeID::CHARGES };
        returnValue << data->_iMaxCharges << " " << spell.getName() << " charges";
      }
        break;
      case ItemEffectID::FIREDAM:
        returnValue << "Fire hit damage: " << data->_iFMinDam << "-" << data->_iFMaxDam;
        break;
      case ItemEffectID::LIGHTDAM:
        returnValue << "Lightning hit damage: " << data->_iLMinDam << "-" << data->_iLMaxDam;
        break;
      case ItemEffectID::STR:
      case ItemEffectID::STR_CURSE:
        if (data->_iPLStr > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << data->_iPLStr << " to strength";
        break;
      case ItemEffectID::MAG:
      case ItemEffectID::MAG_CURSE:
        if (data->_iPLMag > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << data->_iPLMag << " to magic";
        break;
      case ItemEffectID::DEX:
      case ItemEffectID::DEX_CURSE:
        if (data->_iPLDex > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << data->_iPLDex << " to dexterity";
        break;
      case ItemEffectID::VIT:
      case ItemEffectID::VIT_CURSE:
        if (data->_iPLVit > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << data->_iPLVit << " to vitality";
        break;
      case ItemEffectID::ATTRIBS:
      case ItemEffectID::ATTRIBS_CURSE:
        if (data->_iPLStr > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << data->_iPLStr << " to all attributes";
        break;
      case ItemEffectID::GETHIT_CURSE:
      case ItemEffectID::GETHIT:
        returnValue << data->_iPLGetHit << " damage from enemies";
        break;
      case ItemEffectID::LIFE:
      case ItemEffectID::LIFE_CURSE:
        returnValue << "Hit points : ";
        if (data->_iPLHP > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << (data->_iPLHP >> 6);
        break;
      case ItemEffectID::MANA:
      case ItemEffectID::MANA_CURSE:
        returnValue << "Mana : ";
        if (data->_iPLMana > 0)
          returnValue << "+";
        else
          returnValue << "-";
        returnValue << (data->_iPLMana >> 6);
        break;
      case ItemEffectID::DUR:
        returnValue << "high durability";
        break;
      case ItemEffectID::DUR_CURSE:
        returnValue << "decreased durability";
        break;
      case ItemEffectID::INDESTRUCTIBLE:
        returnValue << "indestructible";
        break;
      case ItemEffectID::LIGHT:
        returnValue << "+" << 10 * data->_iPLLight << "% light radius";
        break;
      case ItemEffectID::LIGHT_CURSE:
        returnValue << "-" << -10 * data->_iPLLight << "% light radius";
        break;
      case ItemEffectID::FIRE_ARROWS:
        returnValue << "fire arrows damage: " << data->_iFMinDam << "-" << data->_iFMaxDam;
        break;
      case ItemEffectID::LIGHT_ARROWS:
        returnValue << "lightning arrows damage " << data->_iLMinDam << "-" << data->_iLMaxDam;
        break;
      case ItemEffectID::THORNS:
        returnValue << "attacker takes 1-3 damage";
        break;
      case ItemEffectID::NOMANA:
        returnValue << "user loses all mana";
        break;
      case ItemEffectID::NOHEALPLR:
        returnValue << "you can't heal";
        break;
      case ItemEffectID::ABSHALFTRAP:
        returnValue << "absorbs half of trap damage";
        break;
      case ItemEffectID::KNOCKBACK:
        returnValue << "knocks target back";
        break;
      case ItemEffectID::THREEXDAMVDEM:
        returnValue << "+200% damage vs. demons";
        break;
      case ItemEffectID::ALLRESZERO:
        returnValue << "All Resistance equals 0";
        break;
      case ItemEffectID::NOHEALMON:
        returnValue << "hit monster doesn't heal";
        break;
      case ItemEffectID::STEALMANA:
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::STEALMANA_3))
          returnValue << "hit steals 3% mana";
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::STEALMANA_5))
          returnValue << "hit steals 5% mana";
        break;
      case ItemEffectID::STEALLIFE:
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::STEALLIFE_3))
          returnValue << "hit steals 3% life";
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::STEALLIFE_5))
          returnValue << "hit steals 5% life";
        break;
      case ItemEffectID::TARGAC:
        returnValue << "damages target's armor";
        break;
      case ItemEffectID::FASTATTACK:
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::QUICKATTACK))
          returnValue << "quick attack";
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTATTACK))
          returnValue << "fast attack";
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTERATTACK))
          returnValue << "faster attack";
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTESTATTACK))
          returnValue << "fastest attack";
        break;
      case ItemEffectID::FASTRECOVER:
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTRECOVER))
          returnValue << "fast hit recovery";
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTERRECOVER))
          returnValue << "faster hit recovery";
        if (data->_iFlags & static_cast<int>(ItemSpecialEffect::FASTESTRECOVER))
          returnValue << "fastest hit recovery";
        break;
      case ItemEffectID::FASTBLOCK:
        returnValue << "fast block";
        break;
      case ItemEffectID::DAMMOD:
        returnValue << "adds " << data->_iPLDamMod << " points to damage";
        break;
      case ItemEffectID::RNDARROWVEL:
        returnValue << "fires random speed arrows";
        break;
      case ItemEffectID::SETDAM:
        returnValue << "unusual item damage";
        break;
      case ItemEffectID::SETDUR:
        returnValue << "altered durability";
        break;
      case ItemEffectID::FASTSWING:
        returnValue << "Faster attack swing";
        break;
      case ItemEffectID::ONEHAND:
        returnValue << "one handed sword";
        break;
      case ItemEffectID::DRAINLIFE:
        returnValue << "constantly lose hit points";
        break;
      case ItemEffectID::RNDSTEALLIFE:
        returnValue << "life stealing";
        break;
      case ItemEffectID::NOMINSTR:
        returnValue << "no strength requirement";
        break;
      case ItemEffectID::INFRAVISION:
        returnValue << "see with infravision";
        break;
      case ItemEffectID::INVCURS:
        returnValue << " ";;
        break;
      case ItemEffectID::ADDACLIFE:
        returnValue << "Armor class added to life";
        break;
      case ItemEffectID::ADDMANAAC:
        returnValue << "10% of mana added to armor";
        break;
      case ItemEffectID::FIRERESCLVL:
        if (data->_iPLFR <= 0)
          returnValue << " ";
        else if (data->_iPLFR >= 1)
          returnValue << "Resist Fire : +" << data->_iPLFR << "%";
          break;
      default:
        returnValue << "Another ability (NW)";
        break;
      }
    };
    if (static_cast<ItemQuality>(data->_iMagical) == ItemQuality::NORMAL)
      return returnValue.str();
    if (data->_iPrePower != -1)
    {
      getText(static_cast<ItemEffectID>(data->_iPrePower));
      returnValue << " ";
    }
    if (data->_iSufPower != -1)
      getText(static_cast<ItemEffectID>(data->_iSufPower));

    return returnValue.str();
  }
}