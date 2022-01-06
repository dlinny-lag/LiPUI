#pragma once
#include <vector>
#include "f4se/GameTypes.h"
#include "f4se/ScaleformValue.h"
#include <memory>

template <typename CTX>
struct Request
{
	Request() = default;
	Request(Request&&) = default;
	Request(const Request&) = default;
	Request& operator=(const Request&) = default;
	Request& operator=(Request&&) = default;

	virtual bool Proceed(CTX* context, GFxValue& stage, bool updateContextOnly) const = 0;
	virtual ~Request() = default;
};

template <typename CTX, class REQUEST>
struct RequestsProcessor
{
	RequestsProcessor() = default;
	RequestsProcessor(RequestsProcessor&&) = default;
	RequestsProcessor(const RequestsProcessor&) = delete;
	RequestsProcessor& operator=(const RequestsProcessor&) = delete;
	RequestsProcessor& operator=(RequestsProcessor&&) = default;
	virtual ~RequestsProcessor() = default;

	void Add(REQUEST*&& request)
	{
		BSWriteLocker lock(&queueLock); // I don't think that Add can be invoked during execution of ProcessPendingRequests. just to be sure
		queue.emplace_back(std::unique_ptr<REQUEST>(request));
	}
	void ProcessPendingRequests(CTX* context, GFxValue& stage, bool updateContextOnly)
	{
		BSWriteLocker lock(&queueLock);
		for (const auto& req : queue)
		{
			req->Proceed(context, stage, updateContextOnly);
		}
		queue.clear();
	}
private:
	BSReadWriteLock queueLock;
	std::vector<std::unique_ptr<REQUEST>> queue;
};
