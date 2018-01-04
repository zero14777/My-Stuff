#include "Personality.h"

#include <cstdlib>

namespace game 
{
	Personality::Personality()
	{
		for (uint16_t iter = 0; iter < sizeof(Attribute); iter++)
		{
			m_attributes.emplace_back(rand() % 100);
		}
	}

	Personality::~Personality()
	{
	}

	bool Personality::TestValue(Attribute inAttribute, float inMin, float inMax)
	{
		bool retVal = false;
		
		if (m_attributes.at(inAttribute) > inMin && m_attributes.at(inAttribute) < inMax)
		{
			retVal = true;
		}

		return true;
	}
}
