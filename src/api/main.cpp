#include <iostream>
#include <string>

#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

std::string path = "https://al.sdev.xyz/public/api";
uri_builder api = uri_builder().append_path(U("index.php"));

/*
std::string path = "https://httpbin.org";
uri_builder api = uri_builder().append_path(U("post"));
*/

json::value find(std::string query) {
  query = web::uri::encode_data_string(query);

  json::value out;

  auto requestJson = http_client(U(path))
    .request(methods::POST, api.to_string(),
      U("query=search&search=" + query), U("application/x-www-form-urlencoded"))

  .then([](http_response response) {
    std::cout << "Returned " << std::to_string(response.status_code()) << std::endl;
    return response.extract_json();
  })

  .then([&out](json::value jsonObject) {
    out = jsonObject;
  });

  try {
    requestJson.wait();
  } catch (const std::exception &e) {
    printf("Error exception:%s\n", e.what());
  }

  return out;
}
