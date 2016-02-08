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

#ifndef __PU_EXTERN_FACTORY_H__
#define __PU_EXTERN_FACTORY_H__

#include "ParticleUniverseExtern.h"
#include "ParticleUniverseExternTokens.h"
#include "ParticleUniverseScriptReader.h"

namespace ParticleUniverse {

	/** This is the base factory of all Extern implementations.
    */
	class _ParticleUniverseExport ExternFactory : public ScriptReader, public ScriptWriter, public FactoryAlloc {
	public:
		ExternFactory() {}
		virtual ~ExternFactory() {}

		/** Returns the type of the factory, which identifies the Extern type this factory creates. */
		virtual String getExternType() const = 0;

		/** Creates a new Extern instance.
		@remarks
	    */
		virtual Extern * createExtern() = 0;

		/** Delete an Extern.
	    */
		void destroyExtern(Extern * externObject) {
			if (externObject) {
				PU_DELETE externObject;
			}
		}

	protected:
		/**
		*/
		template<class T>
		Extern * _createExtern() {
			Extern * externObject = PU_NEW T();
			externObject->setExternType(getExternType());
			return externObject;
		}
	};

} /* namespace ParticleUniverse */

#endif /* __PU_EXTERN_FACTORY_H__ */
