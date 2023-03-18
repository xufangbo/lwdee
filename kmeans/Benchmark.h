#ifndef BENCHMARK_H
#define BENCHMARK_H

#pragma once

#include <chrono>
#include <iostream>
#include <string>

class Benchmark
{
public:
	Benchmark(std::string name_) : _name(name_)
	{
		m_begin = std::chrono::system_clock::now();
	};
	~Benchmark()
	{
		auto dur = std::chrono::system_clock::now() - m_begin;
		std::cout << _name << " duration ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << std::endl;
	};
	Benchmark(const Benchmark &) = delete;
	Benchmark &operator=(const Benchmark &) = delete;

private:
	using time_point = decltype(std::chrono::system_clock::now());
	time_point m_begin;
	std::string _name;
};

#endif