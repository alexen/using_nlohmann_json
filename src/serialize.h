#pragma once

#include <stdexcept>

#include <json_types.h>


struct CwtError : std::runtime_error {
     using std::runtime_error::runtime_error;
};
struct CwtParsingError : CwtError {
     using CwtError::CwtError;
};


bool lifetimeIsValid(
     std::time_t iat
     , std::time_t exp
     , std::time_t now = std::time( nullptr )
     , std::time_t acceptableIatDeviation = 100
);


struct Cwt {
     Bytes header;
     Bytes payload;
     Bytes signature;

     static Cwt fromCbor( const Bytes& cbor );
     Bytes toCbor();

     /// Формирует особый CBOR (to-be-signed) для проверки подписи
     Bytes makeTbsBlock() const;
};


struct AuthConsentRequest {
     struct Header {
          static Header fromCbor( const Bytes& cbor );
          Bytes toCbor();

          /// Обязательные стандартизированные параметры заголовка
          std::string typ;
          std::string alg;
          Bytes x5tSt256;

          /// Обязательные нестандартизированные параметры заголовка
          int ver;
          std::string sbt;
     };


     struct Payload {
          static Payload fromCbor( const Bytes& cbor );
          Bytes toCbor();

          /// Случайные числа для защиты от атак межсайтовых запросов и повторного воспроизведения
          Bytes cti;
          Bytes req_cti;

          /// Поля адресации токена
          std::string iss;
          SequenceOf< std::string > aud;

          /// Сроки действия токена
          std::time_t iat = {};
          std::time_t exp = {};

          /// Параметры токена запроса разрешений/согласий конечного пользователя
          std::string client_id;
          std::string client_name;
          std::string client_ogrnip;
          ObjectOf< std::string > resource;
          OptionalObjectOf< std::string > obtained_consent_list;
          OptionalObjectOf< std::string > requested_consent_list;

          /// Обязательные параметры для построения цепочки доверия
          OptionalObjectOf< Bytes > urn_esia_trust;
     };

     Header header;
     Payload payload;

     static AuthConsentRequest fromCbor( const Bytes& headerCbor, const Bytes& payloadCbor );
     static AuthConsentRequest fromCwt( const Cwt& );
};


struct AuthConsentResponse {
     struct Header {
          static Header fromCbor( const Bytes& cbor );
          Bytes toCbor();

          /// Обязательные стандартизированные параметры заголовка
          std::string typ;
          std::string alg;
          Bytes x5c;

          /// Обязательные нестандартизированные параметры заголовка
          int ver;
          std::string sbt;
     };
     struct Payload
     {
          static Payload fromCbor( const Bytes& cbor );
          Bytes toCbor();

          /// Случайные числа для защиты от атак межсайтовых запросов и повторного воспроизведения
          Bytes cti;
          Bytes req_cti;

          /// Поля адресации токена
          std::string iss;
          std::string aud;

          /// Сроки действия токена
          std::time_t iat;
          std::time_t exp;

          /// Обязательные параметры токена ответа на запрос разрешений/согласий конечного пользователя
          Bytes sub;
          std::string client_id;
          SequenceOf< std::string > resource;
          SequenceOf< std::string > responsed_consent_list;

          /// Обязательные параметры для построения цепочки доверия
          ObjectOf< Bytes > urn_esia_trust;
     };

     Header header;
     Payload payload;

     static AuthConsentResponse fromCbor( const Bytes& headerCbor, const Bytes& payloadCbor );
     static AuthConsentResponse fromCwt( const Cwt& );
};
