#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <string>
#include <vector>
#include <optional>
#include <httplib.h>

class NetworkClient {
public:
    explicit NetworkClient(const std::optional<std::string>& custom_ca_cert_path);
    NetworkClient(const NetworkClient& other) = delete;
    NetworkClient(NetworkClient&& other) = delete;
    ~NetworkClient() = default;

    std::vector<std::string> POST_scout(const std::vector<std::string>& libs);
    std::vector<std::string> GET_download(const std::vector<std::string>& libs);

    NetworkClient& operator=(const NetworkClient& other) = delete;
    NetworkClient& operator=(NetworkClient&& other) = delete;
private:
    std::string ca_cert_path_;
};

#endif // NETWORK_CLIENT_H