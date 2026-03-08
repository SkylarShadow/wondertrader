#include "MQManager.h"

#include <spdlog/fmt/fmt.h>

USING_NS_ZTP;

ZTUInt32 MQManager::create_server(const char* url, bool confirm)
{
	MQServerPtr server(new MQServer(this));

	printf("init server\r\n");
	server->init(url, confirm);

	auto id = server->id();

	_servers[id] = server;
	return id;
}

void MQManager::destroy_server(ZTUInt32 id)
{
	auto it = _servers.find(id);
	if(it == _servers.end())
	{
		log_server(id, fmt::format("MQServer {} not exists", id).c_str());
		return;
	}

	_servers.erase(it);
	log_server(id, fmt::format("MQServer {} has been destroyed", id).c_str());
}

void MQManager::publish_message(ZTUInt32 id, const char* topic, const void* data, ZTUInt32 dataLen)
{
	auto it = _servers.find(id);
	if (it == _servers.end())
	{
		log_server(id, fmt::format("MQServer {} not exists", id).c_str());
		return;
	}

	MQServerPtr& server = (MQServerPtr&)it->second;
	server->publish(topic, data, dataLen);
}

void MQManager::log_server(ZTUInt32 id, const char* message)
{
	if (_cb_log)
		_cb_log(id, message, true);
}

void MQManager::log_client(ZTUInt32 id, const char* message)
{
	if (_cb_log)
		_cb_log(id, message, false);
}

ZTUInt32 MQManager::create_client(const char* url, FuncMQCallback cb)
{
	MQClientPtr client(new MQClient(this));
	client->init(url, cb);

	auto id = client->id();

	_clients[id] = client;
	return id;
}

void MQManager::destroy_client(ZTUInt32 id)
{
	auto it = _clients.find(id);
	if (it == _clients.end())
	{
		log_client(id, fmt::format("MQClient {} not exists", id).c_str());
		return;
	}

	_clients.erase(it);
	log_client(id, fmt::format("MQClient {} has been destroyed", id).c_str());
}

void MQManager::sub_topic(ZTUInt32 id, const char* topic)
{
	auto it = _clients.find(id);
	if (it == _clients.end())
	{
		log_client(id, fmt::format("MQClient {} not exists", id).c_str());
		return;
	}

	MQClientPtr& client = (MQClientPtr&)it->second;
	client->sub_topic(topic);
}

void MQManager::start_client(ZTUInt32 id)
{
	auto it = _clients.find(id);
	if (it == _clients.end())
	{
		log_client(id, fmt::format("MQClient {} not exists", id).c_str());
		return;
	}

	MQClientPtr& client = (MQClientPtr&)it->second;
	client->start();
}