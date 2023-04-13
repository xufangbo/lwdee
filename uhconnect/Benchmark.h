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
		running = false;
	};
	~Benchmark()
	{
		auto dur = std::chrono::system_clock::now() - m_begin;
		std::cout << _name << " duration ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << std::endl;
	};
	Benchmark(const Benchmark &) = delete;
	Benchmark &operator=(const Benchmark &) = delete;

	void start() {
		r_begin = std::chrono::system_clock::now();
		running = true;
		total_duration = 0;
	};

	void suspend() {
		if(running) {
			auto dur = std::chrono::system_clock::now() - r_begin;
			total_duration += std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
			running = false;
		};

	};

	void resume() {
		if( !running ) {
			r_begin = std::chrono::system_clock::now();
			running = true;
		};
	};

	void stop() {
		if( running ) {
			suspend();
		}
		std::cout << _name << " duration ms: " << total_duration << std::endl;
	};

private:
	using time_point = decltype(std::chrono::system_clock::now());
	time_point m_begin;
	time_point r_begin;
	// milliseconds_type total_duration;
	int64_t total_duration;
	bool running;
	std::string _name;
};

#endif