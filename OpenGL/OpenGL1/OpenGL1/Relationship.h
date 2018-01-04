#pragma once

#include <vector>

namespace game
{
	class Relationship
	{
	public:

		enum Attribute
		{
			TRUST,
			AFFECTION,
			RESPECT
		};

		Relationship();
		~Relationship();

		bool TestValue(Attribute inAttribute, float inMin, float inMax = 100.0f);
		bool InRelationship();

	private:

		std::vector<float> m_attributes;

		bool m_romance;
	};
}