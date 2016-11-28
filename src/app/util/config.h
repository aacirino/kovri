/**                                                                                           //
 * Copyright (c) 2013-2016, The Kovri I2P Router Project                                      //
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
 *                                                                                            //
 * Parts of the project are originally copyright (c) 2013-2015 The PurpleI2P Project          //
 */

#ifndef SRC_APP_UTIL_CONFIG_H_
#define SRC_APP_UTIL_CONFIG_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <iostream>
#include <map>
#include <string>

#include "core/version.h"
#include "core/util/filesystem.h"

namespace kovri {
namespace app {

/// @enum TunnelsKey
/// @brief Configuration const keys for tunnel config map
enum struct TunnelsKey : std::uint8_t {
  /// @var Type
  /// @brief Key for type of tunnel  (client/server/HTTP, etc.)
  Type,

  /// @var Client
  /// @brief Key for client tunnel
  Client,

  /// @var Server
  /// @brief Key for server tunnel
  Server,

  /// @var HTTP
  /// @brief Key for HTTP tunnel
  HTTP,

  /// @var Address
  /// @brief Key for local client listening address that you'll connect to
  /// @notes Should default to 127.0.0.1
  Address,

  /// @var Dest
  /// @brief Key for I2P hostname or .b32 address
  Dest,

  /// @var DestPort
  /// @brief Key for I2P destination port used in destination
  DestPort,

  /// @var Host
  /// @brief Key for IP address of our local server (that we host)
  /// @notes Should default to 127.0.0.1
  Host,

  /// @var InPort
  /// @brief Key for I2P service port. If unset, should be the same as 'port'
  InPort,

  /// @var ACL
  /// @brief Key for access control list of I2P addresses for server tunnel
  ACL,

  /// @var Port
  /// @brief Key for port of our listening client or server tunnel
  ///   (example: port 80 if you are hosting website)
  Port,

  /// @var Keys
  /// @brief Key for client tunnel identity
  ///   or file with LeaseSet of local service I2P address
  Keys,
};

/// @var TunnelsConfig
/// @brief Map of tunnel config keys to string const
const std::map<TunnelsKey, std::string> TunnelsMap {
  // Section types
  { TunnelsKey::Type, "type" },
  { TunnelsKey::Client, "client" },
  { TunnelsKey::Server, "server" },
  { TunnelsKey::HTTP, "http" },

  // Client-tunnel specific
  { TunnelsKey::Address, "address" },
  { TunnelsKey::Dest, "destination" },
  { TunnelsKey::DestPort, "destinationport" },

  // Server-tunnel specific
  { TunnelsKey::Host, "host" },
  { TunnelsKey::InPort, "inport" },
  { TunnelsKey::ACL, "accesslist" },

  // Tunnel-agnostic
  { TunnelsKey::Port, "port" },
  { TunnelsKey::Keys, "keys" },
};

/// @class Configuration
/// @brief Configuration processing and implementation 
class Configuration {
 public:
  /// @brief Parse command line arguments
  /// @param argc Argument count
  /// @param argv Argument vector
  /// @return False on failure
  bool ParseKovriConfig(
      int argc,
      const char* argv[]);

  /// @brief Parses tunnel configuration file
  void ParseTunnelsConfig();

 private:
  /// @var m_KovriConfig
  /// @brief Variable map for command-line and kovri config file data
  boost::program_options::variables_map m_KovriConfig{};

  /// @class TunnelsConfigSection
  /// @brief Tunnels config file types per section
  struct TunnelsConfigSection {
    TunnelsConfigSection() {}
    std::string name, type, dest, address, keys, host, access_list;
    std::uint16_t port, dest_port, in_port;
  };

  /// @var m_TunnelsConfig
  /// @brief Vector of all sections in a tunnel configuration
  std::vector<TunnelsConfigSection> m_TunnelsConfig{};

 public:
  /// @brief Gets kovri config variable map
  /// @return Reference to kovri config member variable map
  boost::program_options::variables_map& GetParsedKovriConfig() noexcept {
    return m_KovriConfig;
  }

  /// @brief Gets tunnels config member
  /// @return Reference to tunnels config member vector
  std::vector<TunnelsConfigSection>& GetParsedTunnelsConfig() noexcept {
    return m_TunnelsConfig;
  }

  /// @brief Gets complete path + name of kovri config
  /// @return Boost filesystem path of file
  /// @warning Config file must first be parsed
  boost::filesystem::path GetConfigFile() {
    boost::filesystem::path file(
        GetParsedKovriConfig().at("kovriconfig").as<std::string>());
    if (!file.is_complete())
      file = kovri::core::GetDataPath() / file;
    return file;
  }

  /// @brief Gets complete path + name of tunnels config
  /// @return Boost filesystem path of file
  /// @warning Config file must first be parsed
  boost::filesystem::path GetTunnelsConfigFile() {
    boost::filesystem::path file(
        GetParsedKovriConfig().at("tunnelsconf").as<std::string>());
    if (!file.is_complete())
      file = kovri::core::GetDataPath() / file;
    return file;
  }

 private:
  // TODO(unassigned): improve this function and use-case
  /// @brief Parses configuration file and maps options
  /// @param config File name
  /// @param config_options Reference to instantiated options_description
  /// @param var_map Reference to instantiated variables map
  /// @notes command-line opts take precedence over config file opts
  void ParseKovriConfigFile(
      std::string& config,
      boost::program_options::options_description& config_options,
      boost::program_options::variables_map& var_map);

  /// @brief Sets logging options after validating user input
  /// @return False on failure
  /// @notes We set here instead of router context because we start logging
  ///   before router context and client context are initialized
  /// @warning Kovri config must first be parsed
  bool SetLoggingOptions();
};

}  // namespace app
}  // namespace kovri

#endif  // SRC_APP_UTIL_CONFIG_H_
