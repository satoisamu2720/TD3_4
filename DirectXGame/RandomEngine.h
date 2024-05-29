#pragma once
#include <random>
class RandomEngine {
public:
	static RandomEngine* GetInstance() {
		static RandomEngine instance;
		return &instance;
	};
	// void Initialize();
	std::mt19937& GetRandomEngine() { return randomEngine; };
	static float GetRandom(float min, float max) {
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(GetInstance()->GetRandomEngine());
	};

private:
	RandomEngine() {
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		randomEngine = randomEngine;
	};

	~RandomEngine() = default;
	RandomEngine(const RandomEngine&) = delete;
	RandomEngine operator=(const RandomEngine&) = delete;

	std::mt19937 randomEngine_;
};