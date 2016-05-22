/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __PU_CONTEXT_H__
#define __PU_CONTEXT_H__

#include "ParticleUniverseException.h"
#include "ParticleUniverseLogManager.h"
#include "ParticleUniverseSection.h"

namespace ParticleUniverse
{
	// String representation of the Particle Universe contexts. Numeric values are used to keep the string small.
	const static String ALIAS					= "1";
	const static String SYSTEM				= "2";
	const static String TECHNIQUE				= "3";
	const static String RENDERER				= "4";
	const static String EMITTER				= "5";
	const static String AFFECTOR				= "6";
	const static String OBSERVER				= "7";
	const static String HANDLER				= "8";
	const static String BEHAVIOUR				= "9";
	const static String EXTERN				= "10";
	const static String DYNAMIC_ATTRIBUTE		= "11";
	const static String DEPENDENCY			= "12";
	const static String INNER_CONTEXT			= "13"; // To be used for deeper sections

	/** The Context class is used to manage the sections of a particle universe script.
	@remarks
		Each script consists of sections, levels with sublevels and subsublevels, etc. The Context 
		keeps track of each section and during parsing it is clear to which section the script parses 
		is pointing. The Context is used to store crucial data of each section.

		Note, that Context is not used for parsing scripts anymore, but it is still used in writing scripts.
	*/
	class _ParticleUniverseExport Context
	{
		protected:
			typedef vector<Section*> ContextStack;
			ContextStack mContextStack;

			/** This is the last processed section, which is still available for a 'higher' section
			*/

			Section mPreviousProcessedSection;

			/** 
			*/
			Section* _getCurrentSection(void)
			{
				if (!mContextStack.empty())
					return mContextStack.back();
				else
					return 0;
			}

			/** 
			*/
			Section* _getParentSection(void)
			{
				if (mContextStack.size() > 1)
				{
					ContextStack::iterator it = mContextStack.end();
					--it;
					--it;
					return *it;
				}
				else
					return 0;
			}

		public:
			Context(void) : attributeToken(0)
			{
				mPreviousProcessedSection.sectionName = BLANK_STRING;
				mPreviousProcessedSection.element = 0;
				mPreviousProcessedSection.elementName = BLANK_STRING;
			}
			virtual ~Context(void) {}
			
			/** attributeToken is needed to temporary set the last attribute value of the parsed script.
			*/
			IToken* attributeToken;

			/** 
			*/
			void beginSection(
				const String& sectionName, 
				IElement* element = 0,
				const String& elementName = BLANK_STRING)
			{
				// Add a new section to the stack
				Section* section = PU_NEW_T(Section, MEMCATEGORY_SCRIPTING)();
				section->sectionName = sectionName;
				section->elementName = elementName;
				section->element = element;
				mContextStack.push_back(section);
			}
			
			/** 
			*/
			void endSection(void)
			{
				Section* section = _getCurrentSection();

				// Set the values in the mPreviousProcessedSection
				if (section)
				{
					mPreviousProcessedSection.sectionName = section->sectionName;
					mPreviousProcessedSection.element = section->element;
					mPreviousProcessedSection.elementName = section->elementName;
				}
				
				if (!mContextStack.empty())
					mContextStack.pop_back();
				
				if (section)
					PU_DELETE_T(section, Section, MEMCATEGORY_SCRIPTING);
			}
			
			/** 
			*/
			const String& getCurrentSectionName(void)
			{
				Section* section = _getCurrentSection();
				if (section)
					return section->sectionName;
				else
					return BLANK_STRING;
			}

			/** 
			*/
			const String& getPreviousSectionName(void)
			{
				return mPreviousProcessedSection.sectionName;
			}

			/** 
			*/
			const String& getParentSectionName(void)
			{
				Section* parent = _getParentSection();
				if (parent)
					return parent->sectionName;
				else
					return BLANK_STRING;
			}

			/** 
			*/
			IElement* getCurrentSectionElement(void)
			{
				Section* section = _getCurrentSection();
				if (section)
					return section->element;
				else
					return 0;
			}

			/** 
			*/
			IElement* getPreviousSectionElement(void)
			{
				return mPreviousProcessedSection.element;
			}

			/** 
			*/
			IElement* getParentSectionElement(void)
			{
				Section* parent = _getParentSection();
				if (parent)
					return parent->element;
				else
					return 0;
			}

			/** 
			*/
			const String& getCurrentSectionElementName(void)
			{
				Section* section = _getCurrentSection();
				if (section)
					return section->elementName;
				else
					return BLANK_STRING;
			}

			/** 
			*/
			const String& getPreviousSectionElementName(void)
			{
				return mPreviousProcessedSection.elementName;
			}

			/** 
			*/
			const String& getParentSectionElementName(void)
			{
				Section* parent = _getParentSection();
				if (parent)
					return parent->elementName;
				else
					return BLANK_STRING;
			}

			/** 
			*/
			IElement* getSectionElement(const String& sName)
			{
				if (mContextStack.empty())
					return 0;

				ContextStack::iterator it;
				for (it = mContextStack.begin(); it != mContextStack.end(); ++it)
				{
					if ((*it)->sectionName == sName)
					{
						return (*it)->element;
					}
				}
				return 0;
			}

			/** 
			*/
			void validateCurrentSectionName(const String& sectionName, 
				const String& calledFromFunction = BLANK_STRING)
			{
				Section* section = _getCurrentSection();
				if (!section || section->sectionName != sectionName)
				{
					EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid section, expected "  + section->sectionName + 
						" but validated section is " + sectionName, calledFromFunction);
				}
			}
	};

}
#endif
