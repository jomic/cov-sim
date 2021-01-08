#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
namespace demo
{
namespace v1
{
class Test:public drogon::HttpController<Test>
{
  public:
    METHOD_LIST_BEGIN
    //use METHOD_ADD to add your custom processing function here;
    //METHOD_ADD(Test::get,"/{2}/{1}",Get);//path is /demo/v1/Test/{arg2}/{arg1}
    //METHOD_ADD(Test::your_method_name,"/{1}/{2}/list",Get);//path is /demo/v1/Test/{arg1}/{arg2}/list
        METHOD_ADD(Test::login,"/token?userId={1}&passwd={2}",Post);
        METHOD_ADD(Test::getInfo,"/{1}/info?token={2}",Get);
        METHOD_ADD(Test::getInit,"/para?i={1}",Get);
        METHOD_ADD(Test::getMatrixResult,"/matrix/para?init={1}&size={2}&distance={3}",Get);
        METHOD_ADD(Test::getGraphResult,"/{1}/para?init={2}&T={3}",Get);
    //ADD_METHOD_TO(Test::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,int p1,std::string p2);
    // void your_method_name(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,double p1,int p2) const;
    void login(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            std::string &&userId,
            const std::string &password);
    void getInfo(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            std::string userId,
            const std::string &token) const;
    void getInit(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            int i) const;
    void getMatrixResult(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            int i,
            int size,
            int distance) const;
    void getGraphResult(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            std::string type,
            int i,
            int T) const;
};
}
}
