#include "demo_v1_Test.h"
#include <iostream>
#include "../include/Graph.hpp"
#include "../include/Results.hpp"
#include "../include/Simulator.hpp"
#include "../include/Group.hpp"
#include "../include/Agent.hpp"

using namespace demo::v1;
//add definition of your processing function here

void Test::login(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            std::string &&userId,
            const std::string &password)
{
    LOG_DEBUG<<"User "<<userId<<" login";

    Json::Value ret;
    ret["result"]="ok";
    ret["token"]=drogon::utils::getUuid();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Test::getInfo(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            std::string userId,
            const std::string &token) const
{
    LOG_DEBUG<<"User "<<userId<<" get his information";

    Json::Value ret;
    ret["result"]="ok!";
    ret["user_name"]="Jack";
    ret["user_id"]=userId;
    ret["gender"]=1;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Test::setInit(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            int sus,
            int inf,
            int rem)
{
    LOG_DEBUG<<"susceptable init: "<<sus;
    
    Json::Value ret;
    ret["result"]="ok";
    ret["susceptable"]=sus;
    ret["infectious"]=inf;
    ret["removed"]=rem;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Test::getResult(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const 
{
    int L = 10;
    int D_zero = 5;

    srand(1);
    Graph g;
    g.matrix_graph(L, D_zero);

    Simulator s;
    Results results = s.simulator(g);
    results.save_to_file("data.txt");
    results.print(L*L);

    std::vector<result_t> r = results.getResults();
    
    Json::Value ret;
    ret["result"]="ok";
    ret["status"]="final";
    ret["final_susceptable"]=r[r.size()].s;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
