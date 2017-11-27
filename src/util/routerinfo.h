/**                                                                                           //
 * Copyright (c) 2015-2017, The Kovri I2P Router Project                                      //
 *                                                                                            //
 * All rights reserved.                                                                       //
 *                                                                                            //
 * Redistribution and use in source and binary forms, with or without modification, are       //
 * permitted provided that the following conditions are met:                                  //
 *                                                                                            //
 * 1. Redistributions of source code must retain the above copyright notice, this list of     //
 *    conditions and the following disclaimer.                                                //
 *                                                                                            //
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list     //
 *    of conditions and the following disclaimer in the documentation and/or other            //
 *    materials provided with the distribution.                                               //
 *                                                                                            //
 * 3. Neither the name of the copyright holder nor the names of its contributors may be       //
 *    used to endorse or promote products derived from this software without specific         //
 *    prior written permission.                                                               //
 *                                                                                            //
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY        //
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF    //
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL     //
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,       //
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,               //
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    //
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,          //
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF    //
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               //
 */

#ifndef SRC_UTIL_ROUTERINFO_H_
#define SRC_UTIL_ROUTERINFO_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "util/command.h"

#include "core/router/info.h"

/**
 * @class RouterInfoCommand
 * @brief base class for routerinfo
 */

class RouterInfoCommand : public Command, public kovri::core::RouterInfoTraits
{
 public:
  RouterInfoCommand();
  void PrintUsage(const std::string& cmd_name) const;
  bool Impl(const std::string&, const std::vector<std::string>&);
  std::string GetName(void) const
  {
    return "routerinfo";
  }

 private:
  boost::program_options::options_description m_Options;
};

#endif  // SRC_UTIL_ROUTERINFO_H_
