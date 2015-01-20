#pragma once

class Config
{
public:
    Config();
    ~Config();


private:

    std::map<std::string, std::string> _config;
};