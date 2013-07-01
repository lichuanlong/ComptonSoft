/*************************************************************************
 *                                                                       *
 * Copyright (c) 2011 Hirokazu Odaka                                     *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                       *
 *************************************************************************/

#include "EnergySpectrum.hh"

#include <boost/format.hpp>
#include "TDirectory.h"
#include "TH1.h"
#include "CSHitCollection.hh"
#include "InitialInformation.hh"
#include "DetectorHit.hh"

using namespace comptonsoft;
using namespace anl;


EnergySpectrum::EnergySpectrum()
  : m_HitCollection(0), m_InitialInfo(0), m_EnergyBinType("lin"),
    m_NumBinEnergy(720), m_RangeEnergy1(0.0), m_RangeEnergy2(720.0)
{
}


ANLStatus EnergySpectrum::mod_startup()
{
  register_parameter(&m_EnergyBinType, "Energy bin type");
  register_parameter(&m_NumBinEnergy, "Number of bins");
  register_parameter(&m_RangeEnergy1, "Energy min", 1, "keV");
  register_parameter(&m_RangeEnergy2, "Energy max", 1, "keV");
  register_parameter(&m_Selections, "Event selections");
  
  return AS_OK;
}


ANLStatus EnergySpectrum::mod_init()
{
  GetANLModuleNC("CSHitCollection", &m_HitCollection);
  GetANLModuleIFNC("InitialInformation", &m_InitialInfo);
  return AS_OK;
}


ANLStatus EnergySpectrum::mod_his()
{
  VCSModule::mod_his();
  mkdir_module();

  std::vector<double> xs(m_NumBinEnergy+1);
  if (m_EnergyBinType=="log") {
    const double width = std::log(m_RangeEnergy2-m_RangeEnergy1)/m_NumBinEnergy;
    for (size_t k=0; k<xs.size(); k++) {
      xs[k] = m_RangeEnergy1 * std::exp( k*width );
    }
  }

  const size_t n = m_Selections.size();
  for (size_t i=0; i<n; i++) {
    std::string name = (boost::format("hist_%04d") % i).str();
    TH1F* hist = 0;
    if (m_EnergyBinType=="log") {
      hist = new TH1F(name.c_str(), "energy spectrum", m_NumBinEnergy, &xs[0]);
    }
    else {
      hist = new TH1F(name.c_str(), "energy spectrum",
                      m_NumBinEnergy, m_RangeEnergy1, m_RangeEnergy2);
    }
    hist->Sumw2();
    m_Responses[m_Selections[i]] = hist;
  }

  return AS_OK;
}


ANLStatus EnergySpectrum::mod_ana()
{
  typedef std::vector<DetectorHit_sptr> HitVector;

  const double weight = m_InitialInfo->Weight();
  
  HitVector& hitVec = m_HitCollection->GetHits(comptonsoft::NOTIMEGROUP);
  
  double energy = 0.0;
  for (HitVector::iterator it=hitVec.begin(); it!=hitVec.end(); ++it) {
    energy += (*it)->getPI();
  }
  
  for (std::map<std::string, TH1*>::iterator it=m_Responses.begin(); it!=m_Responses.end(); ++it) {
    const std::string& evsName = (*it).first;
    TH1* hist = (*it).second;
    if (Evs(evsName)) {
      hist->Fill(energy/keV, weight);
    }
  }

  return AS_OK;
}
