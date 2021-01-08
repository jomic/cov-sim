#include "demo_v1_Test.h"
#include <fstream>
#include <cstdlib>
#include <iostream>

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
    ret["user_id"]=userId;
    ret["gender"]=1;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Test::getInit(const HttpRequestPtr &req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int i) const 
{
    //system("cd ..");
    //system("cd ..");
    //system("chmod u+x tools/*");

    //std::string initial_json_file = "sw-2Regions.json";
    //std::string result_json_file = "resultMatrix.json";
    //system("tools/compileAndRun cov-sim && bin/cov-sim -iops < " + initial_json_file + " > " + result_json_file);
    //system("tools/compileAndRun cov-sim && bin/cov-sim -iops < sw-2Regions.json > resultMatrix.json");
    system("cd ..&&cd ..&&chmod u+x tools/*&&tools/compileAndRun cov-sim && bin/cov-sim -iops < sw-2Regions.json > resultMatrix.json");
    Json::Value ret;
    std::ifstream config_doc("../../resultMatrix.json", std::ifstream::binary);
    //std::ifstream config_doc("../../"+result_json_file, std::ifstream::binary);
    config_doc >> ret;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);

}

void Test::getMatrixResult(const HttpRequestPtr &req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int i,
        int size,
        int distance) const 
{
    Json::Value root;  
    Json::Reader reader;
    Json::FastWriter fwriter;
    Json::StyledWriter swriter;

    root["select_all"] = true;
    root["initial_infections"] = i;
    root["T"] = 60;
    root["T_v"] = 0;
    root["n_v"] = 0;

    Json::Value group;
    group["n_i"] = 1;
    group["n_ai"] = 0;
    group["susceptibility"] = 1.0;
    group["p_i"] = 0.010417;
    group["p_ai"] = 0.0;
    group["p_t"] = 0.001;
    group["p_at"] = 0.001;
    group["p_v"] = 0;
    group["d_v"] = 20;
    group["d_i"] = 14;
    group["d_ai"] = 12;
    group["a_p"] = 0.0;

    root["groups"].append(group);

    Json::Value graph_1;
    graph_1["type"] = "matrix";
    graph_1["size"] = size;
    graph_1["distance"] = distance;
    
    root["graph"].append(graph_1);

    Json::Value graph_2;
    graph_2["type"] = "matrix";
    graph_2["size"] = size;
    graph_2["distance"] = 23;

    root["graph"].append(graph_2);


    Json::Value t;
    t["type"] = "random";
    root["vaccination_strategy"] = t;

    //Json::Value region_1;
    //Json::value region_2;
    
    Json::Value connection_1 = 1;
    Json::Value connection_2 = 0;

    root["region_connections"].append(connection_1);
    root["region_connections"].append(connection_2);


    std::ofstream ofs;
    ofs.open("../../mx-2Regions.json", std::ios::binary);
    Json::StyledStreamWriter sswriter;
    sswriter.write(ofs, root);
    ofs.close();

    //std::string str = fwriter(root);
    //std::string str = swriter(root);
    //std::ofstream ofs;
    //ofs.open("../../mx-2Regions.json");
    //ofs << str;
    //ofs.close();

    system("cd ..&&cd ..&&chmod u+x tools/*&&tools/compileAndRun cov-sim && bin/cov-sim -iops < mx-2Regions.json > resultMatrix.json");
    Json::Value ret;
    std::ifstream result_doc("../../resultMatrix.json", std::ifstream::binary);
    result_doc >> ret;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Test::getGraphResult(const HttpRequestPtr &req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        std::string type,
        int i,
        int T) const
{
    Json::Value ret;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
