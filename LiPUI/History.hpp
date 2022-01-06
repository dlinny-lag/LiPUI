#pragma once
#include <vector>
#include <limits>

template <typename T, size_t Size>
class History
{
	std::vector<T> buffer;
	size_t start;
public:
	History()
		:start(-1)
	{
		buffer.reserve(Size);
	}
	virtual ~History() = default;
	History(const History& other) = default;
	History(History&& other) = default;
	History& operator=(const History&) = default;
	History& operator=(History&&) = default;

	size_t size() const noexcept
	{
		return buffer.size();
	}
	bool empty() const noexcept
	{
		return buffer.empty();
	}

	T& operator [] (size_t index)
	{
		index = ((index % Size) + Size) % Size;
		return buffer[index];
	}

	void emplace_back(T&& e)
	{
		if (buffer.size() < Size)
			buffer.emplace_back(e);
		else
		{
			buffer[start] = e;
			start++;
			if (start >= Size)
				start = 0;
		}
	}
	void push_back(const T& e)
	{
		T tmp = e;
		emplace_back(std::move(tmp));
	}
};