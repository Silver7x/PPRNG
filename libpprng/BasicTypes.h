/*
  Copyright (C) 2011 chiizu
  chiizu.pprng@gmail.com
  
  This file is part of libpprng.
  
  libpprng is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  libpprng is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with libpprng.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#include <stdint.h>
#include <string>
#include <vector>
#include <stdexcept>

namespace pprng
{

// base exception class for the library
class Exception : public std::runtime_error
{
public:
  Exception(const std::string &what)
    : std::runtime_error(what)
  {}
};

struct DS
{
  enum Type
  {
    Phat,
    Lite,
    i,
    i_XL,
    _3DS,
    
    UnknownDSType
  };
  
  static std::string ToString(Type v);
};


struct Button
{
  enum Binary
  {
    A_BUTTON = 0x1,
    B_BUTTON = 0x2,
    SELECT_BUTTON = 0x4,
    START_BUTTON = 0x8,
    R_BUTTON = 0x100,
    L_BUTTON = 0x200,
    X_BUTTON = 0x400,
    Y_BUTTON = 0x800,
    
    SINGLE_BUTTON_MASK = 0xF0F,
    
    NUM_BINARY_BUTTONS = 8
  };
  
  // treat as one button
  enum DPad
  {
    RIGHT_BUTTON = 0x10,
    LEFT_BUTTON = 0x20,
    UP_BUTTON = 0x40,
    DOWN_BUTTON = 0x80,
    UP_RIGHT = 0x50,
    UP_LEFT = 0x60,
    DOWN_RIGHT = 0x90,
    DOWN_LEFT = 0xA0,
    
    DPAD_MASK = 0xF0,
    DPAD_SHIFT = 4,
    
    NUM_DPAD_DIRECTIONS = 8
  };
  
  typedef std::vector<uint32_t>  List;
  
  static std::string ToString(uint32_t keys);
  
  static const List& SingleButtons();
  static const List& TwoButtonCombos();
  static const List& ThreeButtonCombos();
};

struct Game
{
  enum Version
  {
    ThirdGenStart = 0,
    
    Emerald = ThirdGenStart,
    
    ThirdGenEnd = Emerald + 1,
    
    
    FourthGenStart = ThirdGenEnd,
    
    Diamond = FourthGenStart,
    Pearl,
    Platinum,
    
    HeartGold,
    SoulSilver,
    
    FourthGenEnd = SoulSilver + 1,
    
    
    FifthGenStart = FourthGenEnd,
    
    BlackEnglish = FifthGenStart,
    BlackFrench,
    BlackGerman,
    BlackItalian,
    BlackJapanese,
    BlackSpanish,
    
    WhiteEnglish,
    WhiteFrench,
    WhiteGerman,
    WhiteItalian,
    WhiteJapanese,
    WhiteSpanish,
    
    // add after the fact...
    BlackKorean,
    WhiteKorean,
    
    FifthGenEnd = WhiteKorean + 1,
    
    UnknownVersion = FifthGenEnd,
    
    NumVersions = FifthGenEnd
  };
  
  static std::string ToString(Version v);
};


struct Nature
{
  enum Type
  {
    HARDY = 0,
    LONELY,
    BRAVE,
    ADAMANT,
    NAUGHTY,
    BOLD,
    DOCILE,
    RELAXED,
    IMPISH,
    LAX,
    TIMID,
    HASTY,
    SERIOUS,
    JOLLY,
    NAIVE,
    MODEST,
    MILD,
    QUIET,
    BASHFUL,
    RASH,
    CALM,
    GENTLE,
    SASSY,
    CAREFUL,
    QUIRKY,
    
    NUM_TYPES,
    
    // used when setting search criteria
    ANY = NUM_TYPES,
    
    // returned from FromString if the string given is not a nature
    UNKNOWN
  };
  
  static const std::string& ToString(Type t);
  static Type FromString(const std::string &name);
};


struct Gender
{
  enum Type
  {
    FEMALE = 0,
    MALE,
    NEUTRAL,
    
    ANY = -1
  };
  
  enum Ratio
  {
    ONE_EIGHTH_FEMALE = 0,
    ONE_FOURTH_FEMALE,
    ONE_HALF_FEMALE,
    THREE_FOURTHS_FEMALE,
    FEMALE_ONLY,
    MALE_ONLY,
    
    UNSPECIFIED = -1
  };
  
  static bool GenderValueMatches(uint32_t value, Type t, Ratio r)
  {
    uint32_t  dividingPoint;
    
    if ((t == ANY) || (r == UNSPECIFIED))
      return true;
    
    switch (r)
    {
    case ONE_EIGHTH_FEMALE:
      dividingPoint = 31;
      break;
    case ONE_FOURTH_FEMALE:
      dividingPoint = 63;
      break;
    case ONE_HALF_FEMALE:
      dividingPoint = 127;
      break;
    case THREE_FOURTHS_FEMALE:
      dividingPoint = 191;
      break;
    case UNSPECIFIED:
      return true;
    default:
      return false;
      break;
    }
    
    return (t == FEMALE) ? (value < dividingPoint) : (value >= dividingPoint);
  }
};


struct PersonalityValue
{
  PersonalityValue() : word(0) {}
  
  PersonalityValue(const PersonalityValue &pid) : word(pid.word) {}
  PersonalityValue(uint32_t pidWord) : word(pidWord) {}
  
  PersonalityValue& operator=(const PersonalityValue &pid)
  {
    word = pid.word;
    return *this;
  }
  
  PersonalityValue& operator=(uint32_t pidWord)
  {
    word = pidWord;
    return *this;
  }
  
  uint32_t GenderValue() const { return word & 0xff; }
  
  uint32_t Gen34Ability() const { return word & 0x1; }
  uint32_t Gen5Ability() const { return (word >> 16) & 0x1; }
  
  Nature::Type Gen34Nature() const
  { return static_cast<Nature::Type>(word % 25); }
  
  bool IsShiny(uint32_t tid, uint32_t sid) const
  {
    uint32_t  e = tid ^ sid;
    uint32_t  f = (word >> 16) ^ (word & 0xffff);
    return (e ^ f) < 8;
  }
  
  uint32_t  word;
};

typedef PersonalityValue  PID;


struct Element
{
  enum Type
  {
    NORMAL = 0,
    FIGHTING,
    FLYING,
    POISON,
    GROUND,
    ROCK,
    BUG,
    GHOST,
    STEEL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    PSYCHIC,
    ICE,
    DRAGON,
    DARK,
    
    
    NUM_TYPES,
    
    // used when setting search criteria
    ANY = NUM_TYPES,
    
    // returned from FromString if the string given is not an elemental type
    UNKNOWN
  };
  
  static const std::string& ToString(Type t);
  static Type FromString(const std::string &name);
};


// structure holding the 6 individual values
struct IndividualValues
{
  enum Type
  {
    HP = 0,
    AT,
    DF,
    SA,
    SD,
    SP,
    
    NUM_IVS
  };
  
  // bit shift amounts for packing / unpacking ivs from 32 bit word
  // in Gen 3 / 4 format (empty-bit, sd, sa, sp, empty-bit, df, at, hp)
  enum Shift
  {
    HP_SHIFT = 0,
    AT_SHIFT = 5,
    DF_SHIFT = 10,
    SA_SHIFT = 21,
    SD_SHIFT = 26,
    SP_SHIFT = 16
  };
  
  static const Shift  IVShift[NUM_IVS];
  
  enum Mask
  {
    IV_MASK = 0x1f,
    
    HP_MASK = IV_MASK << HP_SHIFT,
    AT_MASK = IV_MASK << AT_SHIFT,
    DF_MASK = IV_MASK << DF_SHIFT,
    SA_MASK = IV_MASK << SA_SHIFT,
    SD_MASK = IV_MASK << SD_SHIFT,
    SP_MASK = IV_MASK << SP_SHIFT,
    
    ALL_IVS_MASK = HP_MASK | AT_MASK | DF_MASK | SA_MASK | SD_MASK | SP_MASK
  };
  
  static const Mask  IVMask[NUM_IVS];
  
  IndividualValues() : word(0) {}
  
  // this does not check value ranges - assumes user passes valid IV values
  IndividualValues(uint32_t hp, uint32_t at, uint32_t df,
                   uint32_t sa, uint32_t sd, uint32_t sp)
    : word((hp << HP_SHIFT) | (at << AT_SHIFT) | (df << DF_SHIFT) |
           (sa << SA_SHIFT) | (sd << SD_SHIFT) | (sp << SP_SHIFT))
  {}
  
  IndividualValues(const IndividualValues &ivs) : word(ivs.word) {}
  IndividualValues(uint32_t ivWord) : word(ivWord & ALL_IVS_MASK) {}
  
  IndividualValues& operator=(const IndividualValues &ivs)
  {
    word = ivs.word;
    return *this;
  }
  
  IndividualValues& operator=(uint32_t ivWord)
  {
    word = ivWord & ALL_IVS_MASK;
    return *this;
  }
  
  // don't use comparison operators because they will break the rules
  // like if operator< is false, mathematically operator>= should be true
  bool betterThan(const IndividualValues &ivs) const;
  bool betterThanOrEqual(const IndividualValues &ivs) const;
  bool worseThan(const IndividualValues &ivs) const
  { return ivs.betterThan(*this); }
  bool worseThanOrEqual(const IndividualValues &ivs) const
  { return ivs.betterThanOrEqual(*this); }
  
  uint32_t hp() const { return (word & HP_MASK) >> HP_SHIFT; }
  void hp(uint32_t iv)
  { word = (word & ~HP_MASK) | ((iv << HP_SHIFT) & HP_MASK); }
  
  uint32_t at() const { return (word & AT_MASK) >> AT_SHIFT; }
  void at(uint32_t iv)
  { word = (word & ~AT_MASK) | ((iv << AT_SHIFT) & AT_MASK); }
  
  uint32_t df() const { return (word & DF_MASK) >> DF_SHIFT; }
  void df(uint32_t iv)
  { word = (word & ~DF_MASK) | ((iv << DF_SHIFT) & DF_MASK); }
  
  uint32_t sa() const { return (word & SA_MASK) >> SA_SHIFT; }
  void sa(uint32_t iv)
  { word = (word & ~SA_MASK) | ((iv << SA_SHIFT) & SA_MASK); }
  
  uint32_t sd() const { return (word & SD_MASK) >> SD_SHIFT; }
  void sd(uint32_t iv)
  { word = (word & ~SD_MASK) | ((iv << SD_SHIFT) & SD_MASK); }
  
  uint32_t sp() const { return (word & SP_MASK) >> SP_SHIFT; }
  void sp(uint32_t iv)
  { word = (word & ~SP_MASK) | ((iv << SP_SHIFT) & SP_MASK); }
  
  
  class BadIVIndexException : public Exception
  {
  public:
    BadIVIndexException(int i);
  };
  
  uint32_t iv(int i) const throw (BadIVIndexException)
  {
    if ((i < HP) || (i > SP)) throw BadIVIndexException(i);
    
    return (word >> IVShift[i]) & IV_MASK;
  }
  
  void setIV(int i, uint32_t iv) throw (BadIVIndexException)
  {
    if ((i < HP) || (i > SP)) throw BadIVIndexException(i);
    
    word = (word & ~(IV_MASK << IVShift[i])) | ((iv & IV_MASK) << IVShift[i]);
  }
  
  uint32_t Sum() const { return hp() + at() + df() + sa() + sd() + sp(); }
  
  void ShiftDownNormal(uint32_t iv)
  {
    word = ((word & (AT_MASK | DF_MASK)) >> (AT_SHIFT - HP_SHIFT)) |
           ((word & SA_MASK) >> (SA_SHIFT - DF_SHIFT)) |
           ((word & SD_MASK) >> (SD_SHIFT - SA_SHIFT)) |
           ((word & SP_MASK) << (SD_SHIFT - SP_SHIFT)) |
           ((iv & IV_MASK) << SP_SHIFT);
  }
  
  void ShiftUpNormal(uint32_t iv)
  {
    word = ((iv & IV_MASK) << HP_SHIFT) |
           ((word & (HP_MASK | AT_MASK)) << (AT_SHIFT - HP_SHIFT)) |
           ((word & DF_MASK) << (SA_SHIFT - DF_SHIFT)) |
           ((word & SA_MASK) << (SD_SHIFT - SA_SHIFT)) |
           ((word & SD_MASK) >> (SD_SHIFT - SP_SHIFT));
  }
  
  void ShiftDownRoamer(uint32_t iv)
  {
    word = ((word & (AT_MASK | DF_MASK)) >> (AT_SHIFT - HP_SHIFT)) |
           ((word & SD_MASK) >> (SD_SHIFT - DF_SHIFT)) |
           ((word & SP_MASK) << (SD_SHIFT - SP_SHIFT)) |
           ((word & SA_MASK) >> (SA_SHIFT - SP_SHIFT)) |
           ((iv & IV_MASK) << SA_SHIFT);
  }
  
  void ShiftUpRoamer(uint32_t iv)
  {
    word = ((iv & IV_MASK) << HP_SHIFT) |
           ((word & (HP_MASK | AT_MASK)) << (AT_SHIFT - HP_SHIFT)) |
           ((word & DF_MASK) << (SD_SHIFT - DF_SHIFT)) |
           ((word & SD_MASK) >> (SD_SHIFT - SP_SHIFT)) |
           ((word & SP_MASK) << (SA_SHIFT - SP_SHIFT));
  }
  
  Element::Type HiddenType() const;
  uint32_t      HiddenPower() const;
  
  uint32_t  word;
  
  class ImpossibleMinMaxIVRangeException : public Exception
  {
  public:
    ImpossibleMinMaxIVRangeException
      (IndividualValues minIVs, IndividualValues maxIVs);
  };
  
  static uint32_t CalculateNumberOfCombinations
    (IndividualValues minIVs, IndividualValues maxIVs)
    throw (ImpossibleMinMaxIVRangeException)
  {
    if (!minIVs.worseThanOrEqual(maxIVs))
      throw ImpossibleMinMaxIVRangeException(minIVs, maxIVs);
  
    return (maxIVs.hp() - minIVs.hp() + 1) *
           (maxIVs.at() - minIVs.at() + 1) *
           (maxIVs.df() - minIVs.df() + 1) *
           (maxIVs.sa() - minIVs.sa() + 1) *
           (maxIVs.sd() - minIVs.sd() + 1) *
           (maxIVs.sp() - minIVs.sp() + 1);
  }
  
  class ImpossibleHiddenTypeException : public Exception
  {
  public:
    ImpossibleHiddenTypeException
      (IndividualValues minIVs, IndividualValues maxIVs, Element::Type t);
  };
  
  class ImpossibleMinHiddenPowerException : public Exception
  {
  public:
    ImpossibleMinHiddenPowerException
      (IndividualValues minIVs, IndividualValues maxIVs, uint32_t minPower);
  };
  
  static uint64_t AdjustExpectedResultsForHiddenPower
    (uint64_t numResults, IndividualValues minIVs, IndividualValues maxIVs,
     Element::Type type, uint32_t minPower)
    throw (ImpossibleHiddenTypeException, ImpossibleMinHiddenPowerException);
};

typedef IndividualValues  IVs;


inline bool operator==(const IVs &ivs1, const IVs &ivs2)
{ return ivs1.word == ivs2.word; }
inline bool operator!=(const IVs &ivs1, const IVs &ivs2)
{ return ivs1.word != ivs2.word; }



struct Characteristic
{
  enum Type
  {
    // HP
    LOVES_TO_EAT = 0,        // 0 or 5
    OFTEN_DOZES_OFF,         // 1 or 6
    OFTEN_SCATTERS_THINGS,   // 2 or 7
    SCATTERS_THINGS_OFTEN,   // 3 or 8
    LIKES_TO_RELAX,          // 4 or 9
    
    // ATK
    PROUD_OF_ITS_POWER,      // 0 or 5
    LIKES_TO_THRASH_ABOUT,   // 1 or 6
    A_LITTLE_QUICK_TEMPERED, // 2 or 7
    LIKES_TO_FIGHT,          // 3 or 8
    QUICK_TEMPERED,          // 4 or 9
    
    // DEF
    STURDY_BODY,             // 0 or 5
    CAPABLE_OF_TAKING_HITS,  // 1 or 6
    HIGHLY_PERSISTENT,       // 2 or 7
    GOOD_ENDURANCE,          // 3 or 8
    GOOD_PERSEVERANCE,       // 4 or 9
    
    // SPA
    HIGHLY_CURIOUS,          // 0 or 5
    MISCHIEVOUS,             // 1 or 6
    THOROUGHLY_CUNNING,      // 2 or 7
    OFTEN_LOST_IN_THOUGHT,   // 3 or 8
    VERY_FINICKY,            // 4 or 9
    
    // SPD
    STRONG_WILLED,           // 0 or 5
    SOMEWHAT_VAIN,           // 1 or 6
    STRONGLY_DEFIANT,        // 2 or 7
    HATES_TO_LOSE,           // 3 or 8
    SOMEWHAT_STUBBORN,       // 4 or 9
    
    // SPE
    LIKES_TO_RUN,            // 0 or 5
    ALERT_TO_SOUNDS,         // 1 or 6
    IMPETUOUS_AND_SILLY,     // 2 or 7
    SOMEWHAT_OF_A_CLOWN,     // 3 or 8
    QUICK_TO_FLEE,           // 4 or 9
    
    NUM_CHARACTERISTICS,
    
    ANY = NUM_CHARACTERISTICS,
    
    UNKNOWN
  };
  
  static Type Get(PID pid, IVs ivs);
  static const std::string& ToString(Type c);
};


// type of item a wild pokemon might hold
struct HeldItem
{
  enum Type
  {
    NO_ITEM = 0,
    FIFTY_PERCENT_ITEM,
    FIVE_PERCENT_ITEM,
    ONE_PERCENT_ITEM
  };
};


// special parents when breeding
struct FemaleParent
{
  enum Type
  {
    NIDORAN_FEMALE = 0,
    ILLUMISE,
    
    OTHER = -1
  };
};


}

#endif
