#pragma once

#include <stdexcept>

#include "json_types.h"


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
     const Bytes header;
     const Bytes payload;
     const Bytes signature;

     static Cwt getParsed( const Bytes& cbor );

     /// Формирует особый CBOR (to-be-signed) для проверки подписи
     Bytes makeTbsBlock() const;

private:
     explicit Cwt( const Bytes& header, const Bytes& payload, const Bytes& signature )
          : header{ header }, payload{ payload }, signature{ signature }
     {}
};


struct Header
{
     static Header getParsed( const Bytes& cbor );

     /// Сохраненный исходный CBOR для проверки подписи и прочих нужд
     const Bytes raw;

     /// Обязательные стандартизированные параметры заголовка
     std::string typ;
     std::string alg;
     Bytes x5tSt256;

     /// Обязательные нестандартизированные параметры заголовка
     int ver;
     std::string sbt;

private:
     explicit Header( const Bytes& cbor )
          : raw{ cbor }
     {}
};


struct RequestPayload
{
     static RequestPayload getParsed( const Bytes& cbor );

     /// Сохраненный исходный CBOR для проверки подписи и прочих нужд
     const Bytes raw;

     /// Случайные числа для защиты от атак межсайтовых запросов и повторного воспроизведения
     Bytes cti;
     Bytes req_cti;

     /// Поля адресации токена
     std::string iss;
     SequenceOf< std::string > aud;

     /// Сроки действия токена
     std::time_t iat;
     std::time_t exp;

     /// Параметры токена запроса разрешений/согласий конечного пользователя
     std::string client_id;
     std::string client_name;
     std::string client_ogrnip;
     ObjectOf< std::string > resource;
     OptionalObjectOf< std::string > obtained_consent_list;
     OptionalObjectOf< std::string > requested_consent_list;

     /// Обязательные параметры для построения цепочки доверия
     OptionalObjectOf< Bytes > urn_esia_trust;

private:
     explicit RequestPayload( const Bytes& cbor )
          : raw{ cbor }
     {}
};


struct ResponsePayload
{
     static ResponsePayload getParsed( const Bytes& cbor );

     /// Сохраненный исходный CBOR для проверки подписи и прочих нужд
     const Bytes raw;

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
     ObjectOf< std::string > resource;
     OptionalObjectOf< std::string > responsed_consent_list;
     ObjectOf< Bytes > user_device;

     /// Обязательные параметры для построения цепочки доверия
     ObjectOf< Bytes > urn_esia_trust;

private:
     explicit ResponsePayload( const Bytes& cbor )
          : raw{ cbor }
     {}
};
