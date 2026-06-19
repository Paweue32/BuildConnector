#include "network_client.h"

#include <fstream>
#include <filesystem>
#include <future>
#include <indicators/dynamic_progress.hpp>
#include <indicators/progress_bar.hpp>
#include <boost/json.hpp>
#include "utils.h"

std::optional<std::string> find_ca_bundle(const std::optional<std::string>& user_custom_path);
bool download_file(const fs::path& target_dir, const std::string& lib_name, const std::string& ca_cert_path, indicators::DynamicProgress<indicators::ProgressBar>& bar_list, size_t bar_num);


NetworkClient::NetworkClient(const std::optional<std::string>& custom_ca_cert_path) {
    std::optional<std::string> ca_bundle_path = find_ca_bundle(custom_ca_cert_path);
    if(!ca_bundle_path.has_value()) {
        //crash("could not locate a file with trusted certificates. Please use 'buildcon config ca-bundle-path [path]' to set a custom path to the file");
        std::cerr << "could" << std::endl;
        exit(EXIT_FAILURE);
    }

    ca_cert_path_ = ca_bundle_path.value();
}

std::vector<std::string> NetworkClient::POST_scout(const std::vector<std::string>& libs) {
    try {
        httplib::Client cli(constants::server_addr);
        cli.set_ca_cert_path(ca_cert_path_);
        cli.set_connection_timeout(5);
        cli.set_read_timeout(10);
        cli.set_write_timeout(5);

        boost::json::array content;

        content.reserve(libs.size());
        for(const std::string& lib: libs) {
            content.push_back(boost::json::string(lib));
        }

        boost::json::object req_body = {
            {"items", std::move(content)}
        };
        std::string json_payload = boost::json::serialize(std::move(req_body));

        httplib::Result res = cli.Post("/scout", json_payload, "application/json");

        if(!res || res->status != 200) {
            throw 1;
        }

        boost::json::array lib_list = boost::json::parse(res->body).as_object().at("items").as_array();

        std::vector<std::string> final_list;

        final_list.reserve(lib_list.size());
        for(const boost::json::value lib: lib_list) {
            final_list.push_back(std::string(lib.as_string()));
        }

        return final_list;

    } catch(...) {
        crash("failed to fetch dependencies from the server");
    }

    return {};
}

std::vector<std::string> NetworkClient::GET_download(const std::vector<std::string>& libs, const fs::path& target_dir) {
    using namespace indicators;

    std::vector<std::future<bool>> download_threads;
    std::vector<std::string> failures;
    DynamicProgress<ProgressBar> bars;

    show_console_cursor(false);
    download_threads.reserve(libs.size());
    for(size_t i=0;i<libs.size();i++) {
        bars.push_back(
            std::make_unique<ProgressBar>(
                option::BarWidth{50},
                option::ForegroundColor{Color::white},
                option::ShowElapsedTime{false},
                option::ShowRemainingTime{false},
                option::Fill{"="},
                option::PrefixText{std::string("Downloading library ") + libs[i] + ":\t"},
                option::ShowPercentage{true},
                option::FontStyles{std::vector<FontStyle>{indicators::FontStyle::bold}}
            )
        );
        download_threads.push_back(
            std::async(std::launch::async, download_file, target_dir, libs[i], ca_cert_path_, std::ref(bars), i)
        );
    }

    for(int i=0;i<libs.size();i++) {
        if(!download_threads[i].get()) {
            failures.push_back(libs[i]);
        }
    }
    show_console_cursor(true);

    return failures;
}


std::optional<std::string> find_ca_bundle(const std::optional<std::string>& user_custom_path) {
    // 1. User custom path
    if(user_custom_path.has_value() && fs::exists(user_custom_path.value())) {
        return user_custom_path.value();
    }

    // 2. Environment variable
    const char *env_file = std::getenv("SSL_CERT_FILE");
    if(env_file && fs::exists(env_file)) {
        return std::string(env_file);
    }

    // 3. Common locations
    std::vector<std::string> common_paths = {
        "/etc/ssl/certs/ca-certificates.crt",
        "/etc/pki/tls/certs/ca-bundle.crt",
        "/etc/ssl/ca-bundle.pem",
        "/etc/pki/tls/cacert.pem",
        "/etc/ssl/cert.pem",
        "/etc/openssl/certs/ca-certificates.crt",
        "/usr/local/share/certs/ca-root-nss.crt",
        "/etc/ca-certificates/extracted/ca-bundle.trust.crt",
        "/var/lib/ca-certificates/ca-bundle.pem"
    };

    for(const std::string& path: common_paths) {
        if(fs::exists(path)) {
            return path;
        }
    }

    // 4. Well, that's unfortunate
    return std::nullopt;
}

bool download_file(const fs::path& target_dir, const std::string& lib_name, const std::string& ca_cert_path, indicators::DynamicProgress<indicators::ProgressBar>& bar_list, size_t bar_num) {
    if(fs::exists(target_dir / (lib_name + ".tar.gz"))) {
        fs::remove(target_dir / (lib_name + ".tar.gz"));
    }

    httplib::Client cli(constants::server_addr);
    cli.set_ca_cert_path(ca_cert_path);
    cli.set_connection_timeout(5);
    cli.set_read_timeout(10);
    cli.set_write_timeout(5);
    bool everything_ok = 1;
    float percentage = 0;

    auto response_handler = [&everything_ok, &lib_name, &bar_list, bar_num, &percentage](const httplib::Response& response) {
        if(response.status != 200) {
            everything_ok = 0;
            bar_list[bar_num].set_option(indicators::option::PrefixText(lib_name + ": download failed"));
            bar_list[bar_num].set_progress(percentage);
            return false;
        }

        bar_list[bar_num].set_progress(0);
        return true;
    };

    std::ofstream archive_file(target_dir / (lib_name + ".tar.gz"), std::ios::binary);
    if(!archive_file.good()) {
        //crash(std::string("could not create file ") + lib_name + ".tar.gz");
        std::cerr << "Error: could not create file " << lib_name << ".tar.gz" << std::endl;
        return false;
    }

    auto content_receiver = [&everything_ok, &archive_file](const char *data, size_t data_length) {
        archive_file.write(data, data_length);
        return archive_file.good();
    };

    auto download_progress = [&lib_name, &bar_list, bar_num, &percentage](size_t current, size_t total) {
        percentage = floor(100.0*current/total);
        bar_list[bar_num].set_progress(percentage);

        if(current == total) {
            std::string prefix = std::string("Downloaded library ") + lib_name;
            prefix.resize(40, ' ');

            bar_list[bar_num].set_option(indicators::option::PrefixText(prefix));
            bar_list[bar_num].set_progress(100);
        }

        return true;
    };

    httplib::Result res = cli.Get("/download", {{"item", lib_name}}, {}, response_handler, content_receiver, download_progress);

    if(!res) {
        bar_list[bar_num].set_option(indicators::option::PrefixText(lib_name + ": download failed"));
        bar_list[bar_num].set_progress(percentage);
        return false;
    }

    return true;
}

int test() {
    NetworkClient nc(std::nullopt);
    
    std::vector<std::string> final_list = nc.POST_scout({"wt", "httplib", "gmock"});

    for(const auto& s: final_list) {
        std::cout << s << ' ';
    }
    std::cout << std::endl;

    nc.GET_download(final_list, ".");
    return EXIT_SUCCESS;
}