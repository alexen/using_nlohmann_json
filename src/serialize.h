#pragma once

#include "json_types.h"


struct Header
{
     static Header getParsed( Bytes cbor );

     const Bytes raw;

     /// Обязательные стандартизированные параметры заголовка
     std::string typ;
     std::string alg;
     Bytes x5tSt256;

     /// Обязательные нестандартизированные параметры заголовка
     std::string sbt;
     int ver;

private:
     explicit Header( Bytes&& cbor )
          : raw{ std::move( cbor ) }
     {}
};


struct RequestPayload
{
     static RequestPayload getParsed( Bytes cbor );

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
     explicit RequestPayload( Bytes&& cbor )
          : raw{ std::move( cbor ) }
     {}
};


struct ResponsePayload
{
     static ResponsePayload getParsed( Bytes cbor );

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
     OptionalObjectOf< std::string > requested_consent_list;
     ObjectOf< Bytes > user_device;

     /// Обязательные параметры для построения цепочки доверия
     ObjectOf< Bytes > urn_esia_trust;

private:
     explicit ResponsePayload( Bytes&& cbor )
          : raw{ std::move( cbor ) }
     {}
};
