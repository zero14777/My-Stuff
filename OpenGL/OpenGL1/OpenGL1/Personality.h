#pragma once

#include <vector>

namespace game
{
	class Personality
	{
	public:
		enum Attribute
		{
			COURAGE,
			CLASS,
			COMMON_SENSE,
			INNOCENCE,
			ENERGY //depressed - hopeful
		};

		Personality();
		~Personality();

		bool TestValue(Attribute inAttribute, float inMin, float inMax = 100.0f);

	private:
		std::vector<float> m_attributes;
	};
}