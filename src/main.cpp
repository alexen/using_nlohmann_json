#include <iostream>
#include <string_view>

#include <boost/algorithm/hex.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <nlohmann/json.hpp>

#include "serialize.h"


using namespace std::string_literals;


static const auto cbor =
     "835860a563616c676a474f5354333431303132637362747263727970746f5f636f6e73656e"
     "745f726571637479706343575463766572016978357423537432353658201e0e64b7e2210e"
     "34f19293328e3ce191180e39a37e25a9b9f23a724d91f725535901b0ad6361756481666d70"
     "6175746869636c69656e745f69646579612e72756b636c69656e745f6e616d6567636c6965"
     "6e74316d636c69656e745f6f67726e69706d31313332303734353037333535636374695820"
     "911e36fa217419310b0329b04b830a06af76d103a5e9a3223ed24dba92472887636578701a"
     "66fbe6a9636961741a66fbe57d636973737818687474703a2f2f657369612e676f7375736c"
     "7567692e7275756f627461696e65645f636f6e73656e745f6c697374a16866756c6c6e616d"
     "6573d0bfd0bed0bbd0bdd0bed0b520d0b8d0bcd18f677265715f6374695820c5dcb61df15d"
     "70cc17fb6e74f5bcf6710d1ac376d03d9b77c4973f92ba1472c7767265717565737465645f"
     "636f6e73656e745f6c697374a1696269727468646174657819d094d0b0d182d0b020d180d0"
     "bed0b6d0b4d0b5d0bdd0b8d18f687265736f75726365a167736572766572316c7365727665"
     "7231206e616d656e75726e3a657369613a7472757374a1781f75726e3a657369613a747275"
     "73743a63727970746f5f617574685f72657370582021cbb4569de6092fd6ad3968b08f65a0"
     "7798afb8865a5d783e81e4c3a133f70b5840b26fe713f164944f0f5a9e8d8ea99f94a69fc0"
     "f6554f59d56b72f492b97f76c262bdc5f8c627ee9baaf687cc129cc914448b2f3720a02659"
     "16d5ede8efdded21"s;


int main()
{
     try
     {
          const auto parsed = nlohmann::json::from_cbor( boost::algorithm::unhex( cbor ) );
          if( parsed.is_array() and parsed.size() != 3u )
          {
               throw std::invalid_argument{ "bad CWT format" };
          }

          std::cout << std::setw( 2 ) <<
                nlohmann::json::from_cbor( parsed[ 0 ].get_binary() ).get< Header >() << '\n';
          std::cout << std::setw( 2 ) <<
                nlohmann::json::from_cbor( parsed[ 1 ].get_binary() ).get< Payload >() << '\n';

          const auto& signature = parsed[ 2 ].get_binary();
     }
     catch( ... )
     {
          std::cerr << "exception: " << boost::current_exception_diagnostic_information() << '\n';
     }

     return 0;
}
