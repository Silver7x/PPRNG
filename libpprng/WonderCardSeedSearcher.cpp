/*
  Copyright (C) 2011-2012 chiizu
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


#include "WonderCardSeedSearcher.h"
#include "SeedSearcher.h"

namespace pprng
{

namespace
{

struct FrameChecker
{
  FrameChecker(const WonderCardSeedSearcher::Criteria &criteria)
    : m_criteria(criteria)
  {}
  
  bool operator()(const WonderCardFrame &frame) const
  {
    return CheckShiny(frame.pid) && CheckNature(frame.nature) &&
           CheckAbility(frame.pid.Gen5Ability()) &&
           CheckGender(frame.pid.GenderValue()) && CheckIVs(frame.ivs) &&
           CheckHiddenPower(frame.ivs);
  }
  
  bool CheckShiny(PID pid) const
  {
    return !m_criteria.shinyOnly ||
           pid.IsShiny(m_criteria.frameParameters.cardTID,
                       m_criteria.frameParameters.cardSID);
  }
  
  bool CheckNature(Nature::Type nature) const
  {
    return m_criteria.pid.CheckNature(nature);
  }
  
  bool CheckAbility(Ability::Type ability) const
  {
    return (m_criteria.pid.ability == Ability::ANY) ||
           (m_criteria.pid.ability == ability);
  }
  
  bool CheckGender(uint32_t genderValue) const
  {
    return Gender::GenderValueMatches(genderValue,
                                      m_criteria.pid.gender,
                                      m_criteria.pid.genderRatio);
  }
  
  bool CheckIVs(const IVs &ivs) const
  {
    return ivs.betterThanOrEqual(m_criteria.ivs.min) &&
           (m_criteria.ivs.max.isMax() ||
            ivs.worseThanOrEqual(m_criteria.ivs.max));
  }

  bool CheckHiddenPower(const IVs &ivs) const
  {
    return m_criteria.ivs.CheckHiddenPower(ivs.HiddenType(), ivs.HiddenPower());
  }
  
  const WonderCardSeedSearcher::Criteria  &m_criteria;
};

struct FrameGeneratorFactory
{
  typedef WonderCardFrameGenerator  FrameGenerator;
  
  FrameGeneratorFactory(const WonderCardSeedSearcher::Criteria &criteria)
    : m_criteria(criteria)
  {}
  
  WonderCardFrameGenerator operator()(const HashedSeed &seed) const
  {
    return WonderCardFrameGenerator(seed, m_criteria.frameParameters);
  }
  
  const WonderCardSeedSearcher::Criteria  &m_criteria;
};

}

uint64_t WonderCardSeedSearcher::Criteria::ExpectedNumberOfResults() const
{
  uint64_t  numSeeds = seedParameters.NumberOfSeeds();
  
  uint64_t  numFrames = frame.max - frame.min + 1;
  
  uint64_t  numIVs = IVs::CalculateNumberOfCombinations(ivs.min, ivs.max);
  
  uint64_t  natureMultiplier = pid.NumNatures(), natureDivisor = 25;
  uint64_t  abilityDivisor = (pid.ability > 1) ? 1 : 2;
  
  uint64_t  shinyDivisor = (shinyOnly &&
                            (frameParameters.cardShininess ==
                             WonderCardShininess::MAY_BE_SHINY)) ? 8192 : 1;
  
  uint64_t  numResults = numSeeds * numFrames * numIVs * natureMultiplier /
                         (32 * 32 * 32 * 32 * 32 * 32 *
                          natureDivisor * abilityDivisor * shinyDivisor);
  
  numResults = IVs::AdjustExpectedResultsForHiddenPower
    (numResults, ivs.min, ivs.max, ivs.hiddenTypeMask, ivs.minHiddenPower);
  
  return numResults;
}

void WonderCardSeedSearcher::Search
  (const Criteria &criteria, const ResultCallback &resultHandler,
   const SearchRunner::ProgressCallback &progressHandler)
{
  HashedSeedGenerator         seedGenerator(criteria.seedParameters);
  FrameGeneratorFactory       frameGeneratorFactory(criteria);
  
  // slightly hacky...
  SearchCriteria::FrameRange  frameRange
    (criteria.frameParameters.startFromLowestFrame ? 1 : criteria.frame.min,
     criteria.frame.max);
  
  SeedFrameSearcher<FrameGeneratorFactory>  seedSearcher(frameGeneratorFactory,
                                                         frameRange);
  
  FrameChecker                frameChecker(criteria);
  
  SearchRunner                searcher;
  
  searcher.SearchThreaded(seedGenerator, seedSearcher, frameChecker,
                          resultHandler, progressHandler);
}

}
