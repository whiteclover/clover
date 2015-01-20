#include "HttpUtil.h"


void EncodeUri(std::string &uri)
{
    char * tmp = evhttp_encode_uri(uri.c_str());
    uri.assign(tmp);
    free(tmp);
}


void AddReqParam(std::string &data, const std::string &key, const std::string &value)
{
    if (value.empty())
    {
        return ;
    }

    if (!data.empty())
    {
        data.append("&");
    }

    data.append(key);
    data.append("=");
    char * tmp = evhttp_encode_uri(value.c_str());
    data.append(tmp);
    free(tmp);
}