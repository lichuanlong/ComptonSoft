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

#ifndef COMPTONSOFT_WriteXrayEventTree_H
#define COMPTONSOFT_WriteXrayEventTree_H 1

#include "VCSModule.hh"
#include <memory>

class TTree;

namespace anlgeant4 {
class InitialInformation;
}

namespace comptonsoft {

class XrayEventTreeIO;
class XrayEventCollection;

/**
 * 
 * @author Hirokazu Odaka
 * @date 2019-06-05
 * @date 2019-10-30 | use XrayEventCollection
 */
class WriteXrayEventTree : public VCSModule
{
  DEFINE_ANL_MODULE(WriteXrayEventTree, 1.1);
public:
  WriteXrayEventTree();
  ~WriteXrayEventTree() = default;

  anlnext::ANLStatus mod_define() override;
  anlnext::ANLStatus mod_initialize() override;
  anlnext::ANLStatus mod_analyze() override;

private:
  std::string collectionModule_;
  const XrayEventCollection* collection_ = nullptr;
  TTree* tree_ = nullptr;
  std::unique_ptr<XrayEventTreeIO> treeIO_ = nullptr;
};

} /* namespace comptonsoft */

#endif /* COMPTONSOFT_WriteHitTree_H */
