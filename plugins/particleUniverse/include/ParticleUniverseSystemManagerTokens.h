/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_SYSTEM_MANAGER_TOKENS_H__
#define __PU_SYSTEM_MANAGER_TOKENS_H__

#include "ParticleUniverseAffectorTokens.h"
#include "ParticleUniverseAliasTokens.h"
#include "ParticleUniverseBehaviourTokens.h"
#include "ParticleUniverseEmitterTokens.h"
#include "ParticleUniverseEventHandlerTokens.h"
#include "ParticleUniverseExternTokens.h"
#include "ParticleUniverseObserverTokens.h"
#include "ParticleUniverseRendererTokens.h"
#include "ParticleUniverseSystemTokens.h"
#include "ParticleUniverseTechniqueTokens.h"

namespace ParticleUniverse {

	/** The BuiltinScriptTranslatorManager contains all Translators that are used by the compiler.
	*/
	class _ParticleUniverseExport BuiltinScriptTranslatorManager : public Ogre::ScriptTranslatorManager {
	public:
		BuiltinScriptTranslatorManager();

		/** 
		*/ 
		virtual size_t getNumTranslators() const;

		/** 
		*/ 
		virtual ScriptTranslator * getTranslator(const AbstractNodePtr & node);

	private:
		SystemTranslator mSystemTranslator;
		AliasTranslator mAliasTranslator;
		TechniqueTranslator mTechniqueTranslator;
		RendererTranslator mRendererTranslator;
		EmitterTranslator mEmitterTranslator;
		AffectorTranslator mAffectorTranslator;
		BehaviourTranslator mBehaviourTranslator;
		ObserverTranslator mObserverTranslator;
		ParticleEventHandlerTranslator mParticleEventHandlerTranslator;
		ExternTranslator mExternTranslator;
	};

	/** 
    */
	class _ParticleUniverseExport ParticleSystemManagerWriter : public ScriptWriter {
	public:
		ParticleSystemManagerWriter() {}
		virtual ~ParticleSystemManagerWriter() {}

		/** @see
			ScriptWriter::write
		*/
		virtual void write(ParticleScriptSerializer * serializer, const IElement * element) {}
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SYSTEM_MANAGER_TOKENS_H__ */
