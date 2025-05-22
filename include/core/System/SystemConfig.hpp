#pragma once

#include <core/logger.hpp>

#include <filesystem>
#include <fstream>
#include <string>
#include <stdio.h> 

#include <jsoncpp/json/json.h>

#ifndef CONFIG_FILE_PATH
#define CONFIG_FILE_PATH "config.json"
#endif /* CONFIG_FILE_PATH */

#ifndef DEFAULT_CONFIG_FILE_PATH
#define DEFAULT_CONFIG_FILE_PATH "config.default.json"
#endif /* DEFAULT_CONFIG_FILE_PATH */



namespace fs = std::filesystem;

namespace kuro {
    namespace System {

        class SystemConfig
        {
        private:
            bool conf_file_exist_ = false;
            std::string conf_path_ = "config.json";

            void copy_config_json_with_default() {
                fs::path default_conf = DEFAULT_CONFIG_FILE_PATH;
                
                if (!fs::exists(default_conf)) {
                    throw std::runtime_error("Missing config.default.json!");
                }

                Logger::log(Logger::Level::Warning, "Created new config.json from template");
                fs::copy(default_conf, conf_path_);
            }
            void config_json_is_exist() {
                conf_path_ = CONFIG_FILE_PATH;
                Logger::log(Logger::Level::Debug, "Watch in " + conf_path_);
                if (fs::exists(conf_path_)) {
                    conf_file_exist_ = true;
                    return;
                }
                
                copy_config_json_with_default();
                conf_file_exist_ = true;
            }
            static SystemConfig& inside_instance() {
                static SystemConfig instance;
                return instance;
            }
            
        public:
            SystemConfig() = default;
            ~SystemConfig() = default;

            static std::string get_parameter_value_str(const std::string& param) {
                if(!inside_instance().conf_file_exist_) {
                    inside_instance().config_json_is_exist();
                    if(!inside_instance().conf_file_exist_) {
                        throw std::runtime_error("Config file not found and can't be created");
                    }
                }

                std::ifstream f(inside_instance().conf_path_);
                if (!f.is_open()) {
                    throw std::runtime_error("Can't open config file");
                }

                Json::Value root;
                Json::CharReaderBuilder builder;
                std::string errs;
                if (!Json::parseFromStream(builder, f, &root, &errs)) {
                    throw std::runtime_error("JSON parse error: " + errs);
                }

                std::vector<std::string> parts;
                size_t start = 0;
                size_t end = param.find('.');
                
                while (end != std::string::npos) {
                    parts.push_back(param.substr(start, end - start));
                    start = end + 1;
                    end = param.find('.', start);
                }
                parts.push_back(param.substr(start));

                Json::Value current = root;
                for (const auto& part : parts) {
                    if (!current.isMember(part)) {
                        Logger::log(Logger::Level::Error, std::string(__FILE__ ) + ": on line " + std::to_string(__LINE__) + ": Param not foud: " + part);
                        throw std::runtime_error("KuroNet System Error");
                    }
                    current = current[part];
                }

                if (current.isString()) {
                    return current.asString();
                } else {
                    Json::StreamWriterBuilder writer;
                    return Json::writeString(writer, current);
                }
            }
            static int get_parameter_value_int(const std::string& param) {
                auto root = get_parameter_value_str(param);
                return std::stoi(root);
            }

        };

    } // namespace System
} // namespace kuro