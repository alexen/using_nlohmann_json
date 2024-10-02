#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/algorithm/hex.hpp>

#include <serialize.h>


std::string asHex( const Bytes& bytes )
{
     static std::ostringstream oss;
     oss.str( {} );
     boost::algorithm::hex( bytes.cbegin(), bytes.cend(),
          std::ostreambuf_iterator< char >( oss ) );
     return oss.str();
}


BOOST_TEST_DONT_PRINT_LOG_VALUE( Bytes );
BOOST_AUTO_TEST_SUITE( CryptoConsentCborTools )
BOOST_AUTO_TEST_CASE( IsExpiredFunction )
{
     std::time_t iat, exp, now;

     /// OK: Токен жив: iat < now < exp
     std::tie( iat, now, exp ) = std::make_tuple( 1000, 1500, 2000 );
     BOOST_TEST( lifetimeIsValid( iat, exp, now ) );

     /// BAD! Создан в будущем: now(!) << iat < exp
     std::tie( iat, now, exp ) = std::make_tuple( 1000,  500, 2000 );
     BOOST_TEST( not lifetimeIsValid( iat, exp, now ) );

     /// BAD! Срок действия истек: iat < exp << now(!)
     std::tie( iat, now, exp ) = std::make_tuple( 1000, 2500, 2000 );
     BOOST_TEST( not lifetimeIsValid( iat, exp, now ) );

     /// BAD! Срок действия истек: iat < exp == now(!)
     std::tie( iat, now, exp ) = std::make_tuple( 1000, 2500, 2500 );
     BOOST_TEST( not lifetimeIsValid( iat, exp, now ) );

     /// Допустимое отклонение iat
     const std::size_t iatDev = 10;

     /// OK: Создан в допустимом будущем: now + 9 сек
     std::tie( iat, now, exp ) = std::make_tuple( 1000 + (iatDev - 1), 1000, 2000 );
     BOOST_TEST( lifetimeIsValid( iat, exp, now, iatDev ) );

     /// OK: Создан точно на границе допустимого: now + 10 сек
     std::tie( iat, now, exp ) = std::make_tuple( 1000 + iatDev, 1000, 2000 );
     BOOST_TEST( lifetimeIsValid( iat, exp, now, iatDev ) );
}
BOOST_AUTO_TEST_SUITE_END() /// CryptoConsentCborTools
BOOST_AUTO_TEST_SUITE( CryptoConsentCborParsing )
BOOST_AUTO_TEST_CASE( ValidCborToCwt )
{
     static const std::string cborHex =
          "835860a563616c676a474f5354333431303132637362747263727970746f"
          "5f636f6e73656e745f726571637479706343575463766572016978357423"
          "537432353658201e0e64b7e2210e34f19293328e3ce191180e39a37e25a9"
          "b9f23a724d91f725535901b0ad6361756481666d706175746869636c6965"
          "6e745f69646579612e72756b636c69656e745f6e616d6567636c69656e74"
          "316d636c69656e745f6f67726e69706d3131333230373435303733353563"
          "6374695820911e36fa217419310b0329b04b830a06af76d103a5e9a3223e"
          "d24dba92472887636578701a66fbe6a9636961741a66fbe57d6369737378"
          "18687474703a2f2f657369612e676f7375736c7567692e7275756f627461"
          "696e65645f636f6e73656e745f6c697374a16866756c6c6e616d6573d0bf"
          "d0bed0bbd0bdd0bed0b520d0b8d0bcd18f677265715f6374695820c5dcb6"
          "1df15d70cc17fb6e74f5bcf6710d1ac376d03d9b77c4973f92ba1472c776"
          "7265717565737465645f636f6e73656e745f6c697374a169626972746864"
          "6174657819d094d0b0d182d0b020d180d0bed0b6d0b4d0b5d0bdd0b8d18f"
          "687265736f75726365a167736572766572316c73657276657231206e616d"
          "656e75726e3a657369613a7472757374a1781f75726e3a657369613a7472"
          "7573743a63727970746f5f617574685f72657370582021cbb4569de6092f"
          "d6ad3968b08f65a07798afb8865a5d783e81e4c3a133f70b5840b26fe713"
          "f164944f0f5a9e8d8ea99f94a69fc0f6554f59d56b72f492b97f76c262bd"
          "c5f8c627ee9baaf687cc129cc914448b2f3720a0265916d5ede8efdded21";

     const auto cborBin = boost::algorithm::unhex( cborHex );

     BOOST_REQUIRE_NO_THROW( Cwt::fromCbor({ cborBin.cbegin(), cborBin.cend() }) );
}
BOOST_AUTO_TEST_CASE( InvalidCborToCwt )
{
     const std::string cborBin = "Not a CBOR at all!";

     BOOST_REQUIRE_THROW( Cwt::fromCbor({ cborBin.cbegin(), cborBin.cend() }), CwtError );
}
BOOST_AUTO_TEST_CASE( ParseCryptoConsentRequestCwt )
{
     static const std::string cborHex =
          "835860a563616c676a474f5354333431303132637362747263727970746f"
          "5f636f6e73656e745f726571637479706343575463766572016978357423"
          "537432353658201e0e64b7e2210e34f19293328e3ce191180e39a37e25a9"
          "b9f23a724d91f725535901b0ad6361756481666d706175746869636c6965"
          "6e745f69646579612e72756b636c69656e745f6e616d6567636c69656e74"
          "316d636c69656e745f6f67726e69706d3131333230373435303733353563"
          "6374695820911e36fa217419310b0329b04b830a06af76d103a5e9a3223e"
          "d24dba92472887636578701a66fbe6a9636961741a66fbe57d6369737378"
          "18687474703a2f2f657369612e676f7375736c7567692e7275756f627461"
          "696e65645f636f6e73656e745f6c697374a16866756c6c6e616d6573d0bf"
          "d0bed0bbd0bdd0bed0b520d0b8d0bcd18f677265715f6374695820c5dcb6"
          "1df15d70cc17fb6e74f5bcf6710d1ac376d03d9b77c4973f92ba1472c776"
          "7265717565737465645f636f6e73656e745f6c697374a169626972746864"
          "6174657819d094d0b0d182d0b020d180d0bed0b6d0b4d0b5d0bdd0b8d18f"
          "687265736f75726365a167736572766572316c73657276657231206e616d"
          "656e75726e3a657369613a7472757374a1781f75726e3a657369613a7472"
          "7573743a63727970746f5f617574685f72657370582021cbb4569de6092f"
          "d6ad3968b08f65a07798afb8865a5d783e81e4c3a133f70b5840b26fe713"
          "f164944f0f5a9e8d8ea99f94a69fc0f6554f59d56b72f492b97f76c262bd"
          "c5f8c627ee9baaf687cc129cc914448b2f3720a0265916d5ede8efdded21";

     const auto cborBin = boost::algorithm::unhex( cborHex );

     const auto cwt = Cwt::fromCbor({ cborBin.cbegin(), cborBin.cend() });
     const auto request = AuthConsentRequest::fromCwt( cwt );

     BOOST_TEST( request.header.typ               == "CWT" );
     BOOST_TEST( request.header.alg               == "GOST341012" );
     BOOST_TEST( asHex( request.header.x5tSt256 ) == "1E0E64B7E2210E34F19293328E3CE191180E39A37E25A9B9F23A724D91F72553" );
     BOOST_TEST( request.header.sbt               == "crypto_consent_req" );
     BOOST_TEST( request.header.ver               == 1 );
     BOOST_TEST( asHex( request.payload.cti )                                                    == "911E36FA217419310B0329B04B830A06AF76D103A5E9A3223ED24DBA92472887" );
     BOOST_TEST( asHex( request.payload.req_cti )                                                == "C5DCB61DF15D70CC17FB6E74F5BCF6710D1AC376D03D9B77C4973F92BA1472C7" );
     BOOST_TEST( request.payload.iss                                                             == "http://esia.gosuslugi.ru" );
     BOOST_TEST( request.payload.aud.size()                                                      == 1u );
     BOOST_TEST( request.payload.aud[ 0 ]                                                        == "mpauth" );
     BOOST_TEST( request.payload.iat                                                             == 1727784317 );
     BOOST_TEST( request.payload.exp                                                             == 1727784617 );
     BOOST_TEST( request.payload.client_id                                                       == "ya.ru" );
     BOOST_TEST( request.payload.client_name                                                     == "client1" );
     BOOST_TEST( request.payload.client_ogrnip                                                   == "1132074507355" );
     BOOST_TEST( request.payload.resource.size()                                                 == 1u );
     BOOST_TEST( request.payload.resource.count( "server1" )                                     != 0u );
     BOOST_TEST( request.payload.resource.at( "server1" )                                        == "server1 name" );
     BOOST_TEST( !!request.payload.obtained_consent_list );
     BOOST_TEST( request.payload.obtained_consent_list->size()                                   == 1u );
     BOOST_TEST( request.payload.obtained_consent_list->count( "fullname" )                      != 0u );
     BOOST_TEST( request.payload.obtained_consent_list->at( "fullname" )                         == "полное имя" );
     BOOST_TEST( !!request.payload.requested_consent_list );
     BOOST_TEST( request.payload.requested_consent_list->size()                                  == 1u );
     BOOST_TEST( request.payload.requested_consent_list->count( "birthdate" )                    != 0u );
     BOOST_TEST( request.payload.requested_consent_list->at( "birthdate" )                       == "Дата рождения" );
     BOOST_TEST( !!request.payload.urn_esia_trust );
     BOOST_TEST( request.payload.urn_esia_trust->size()                                          == 1u );
     BOOST_TEST( request.payload.urn_esia_trust->count( "urn:esia:trust:crypto_auth_resp" )      != 0u );
     BOOST_TEST( asHex( request.payload.urn_esia_trust->at( "urn:esia:trust:crypto_auth_resp" ) ) == "21CBB4569DE6092FD6AD3968B08F65A07798AFB8865A5D783E81E4C3A133F70B" );

     BOOST_TEST( asHex( cwt.signature ) == "B26FE713F164944F0F5A9E8D8EA99F94A69FC0F6554F59D56B72F492B97F76C262BDC5F8C627EE9BAAF687CC129CC914448B2F3720A0265916D5EDE8EFDDED21" );
}
BOOST_AUTO_TEST_CASE( CreateTbsCbor )
{
     static const std::string cborHex =
          "835860a563616c676a474f5354333431303132637362747263727970746f"
          "5f636f6e73656e745f726571637479706343575463766572016978357423"
          "537432353658201e0e64b7e2210e34f19293328e3ce191180e39a37e25a9"
          "b9f23a724d91f725535901b0ad6361756481666d706175746869636c6965"
          "6e745f69646579612e72756b636c69656e745f6e616d6567636c69656e74"
          "316d636c69656e745f6f67726e69706d3131333230373435303733353563"
          "6374695820911e36fa217419310b0329b04b830a06af76d103a5e9a3223e"
          "d24dba92472887636578701a66fbe6a9636961741a66fbe57d6369737378"
          "18687474703a2f2f657369612e676f7375736c7567692e7275756f627461"
          "696e65645f636f6e73656e745f6c697374a16866756c6c6e616d6573d0bf"
          "d0bed0bbd0bdd0bed0b520d0b8d0bcd18f677265715f6374695820c5dcb6"
          "1df15d70cc17fb6e74f5bcf6710d1ac376d03d9b77c4973f92ba1472c776"
          "7265717565737465645f636f6e73656e745f6c697374a169626972746864"
          "6174657819d094d0b0d182d0b020d180d0bed0b6d0b4d0b5d0bdd0b8d18f"
          "687265736f75726365a167736572766572316c73657276657231206e616d"
          "656e75726e3a657369613a7472757374a1781f75726e3a657369613a7472"
          "7573743a63727970746f5f617574685f72657370582021cbb4569de6092f"
          "d6ad3968b08f65a07798afb8865a5d783e81e4c3a133f70b5840b26fe713"
          "f164944f0f5a9e8d8ea99f94a69fc0f6554f59d56b72f492b97f76c262bd"
          "c5f8c627ee9baaf687cc129cc914448b2f3720a0265916d5ede8efdded21";

     const auto cborBin = boost::algorithm::unhex( cborHex );

     const auto cwt = Cwt::fromCbor({ cborBin.cbegin(), cborBin.cend() });
     const auto tbsBlock = cwt.makeTbsBlock();
     BOOST_TEST( asHex( tbsBlock ) == "846A5369676E6174757265315860A563616C676A474F5354333431303132637362747263727970746F5F636F6E73656E745F726571637479706343575463766572016978357423537432353658201E0E64B7E2210E34F19293328E3CE191180E39A37E25A9B9F23A724D91F72553405901B0AD6361756481666D706175746869636C69656E745F69646579612E72756B636C69656E745F6E616D6567636C69656E74316D636C69656E745F6F67726E69706D31313332303734353037333535636374695820911E36FA217419310B0329B04B830A06AF76D103A5E9A3223ED24DBA92472887636578701A66FBE6A9636961741A66FBE57D636973737818687474703A2F2F657369612E676F7375736C7567692E7275756F627461696E65645F636F6E73656E745F6C697374A16866756C6C6E616D6573D0BFD0BED0BBD0BDD0BED0B520D0B8D0BCD18F677265715F6374695820C5DCB61DF15D70CC17FB6E74F5BCF6710D1AC376D03D9B77C4973F92BA1472C7767265717565737465645F636F6E73656E745F6C697374A1696269727468646174657819D094D0B0D182D0B020D180D0BED0B6D0B4D0B5D0BDD0B8D18F687265736F75726365A167736572766572316C73657276657231206E616D656E75726E3A657369613A7472757374A1781F75726E3A657369613A74727573743A63727970746F5F617574685F72657370582021CBB4569DE6092FD6AD3968B08F65A07798AFB8865A5D783E81E4C3A133F70B" );
}
BOOST_AUTO_TEST_SUITE_END() /// CryptoConsentCborParsing
