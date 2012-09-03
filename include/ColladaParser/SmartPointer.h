/*
Copyright (c) 2010 Goran Sterjov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef COLLADA_PARSER_SMART_POINTER_H_
#define COLLADA_PARSER_SMART_POINTER_H_


#include <cassert>

#include <ColladaParser/Config.h>


namespace ColladaParser
{

	/**
	 * A reference counter for intrusive smart pointers.
	 * 
	 * All classes that want to benefit from the smart pointer implementation
	 * of Ref and WeakRef must inherit from ReferenceCounter in order to
	 * provide a counting mechanism for the intrusive smart pointer
	 * implementation.
	 */
	class COLLADA_PARSER_API ReferenceCounter
	{
		/* only Ref is allowed to access internal counter implementation */
		template<class T> friend class Ref;
		
		
	public:
		ReferenceCounter() : mCount(0) {}
		virtual ~ReferenceCounter() {}
		
		
	private:
		int mCount;
		
		/* counter operations */
		void increment() { ++mCount; }
		void decrement() { --mCount; }
		
		/* get current reference count */
		int value() { return mCount; }
	};
	
	
	
	
	/**
	 * Strong intrusive smart pointer.
	 * 
	 * Ref maintains a strong reference to a pointer. Its main purpose
	 * is memory management in that the stored pointer is freed once it is no
	 * longer being referenced. It also manages the reference counter whenever
	 * the reference is copied over or released. Unlike WeakRef, no
	 * precaution is needed to ensure the validity of the stored pointer as it
	 * will always exist.
	 * 
	 * Due to the smart pointer being intrusive we can free the reference
	 * through a virtual base destructor which allows us to use the smart
	 * pointer with incomplete types.
	 */
	template <class T>
	class COLLADA_PARSER_API Ref
	{
	public:
		/**
		 * Pointer constructor.
		 * 
		 * A constructor which stores the specified pointer and increments
		 * the associated counter. As a result the pointer must inherit from
		 * ReferenceCounter.
		 * 
		 * @param ptr The pointer to maintain.
		 */
		explicit Ref (T* ptr) : mPointer(ptr), mCounter(ptr)
		{
			/* increase reference */
			if (mCounter) mCounter->increment();
		}
		
		
		/**
		 * Copy constructor.
		 * @param ref The smart pointer to copy.
		 */
		Ref (const Ref& ref) : mPointer(ref.mPointer), mCounter(ref.mCounter)
		{
			/* increase reference */
			if (mCounter) mCounter->increment();
		}
		
		
		/**
		 * Implicit conversion between smart pointers.
		 * 
		 * Without this we wont be able to cast a pointer to its base
		 * type and still keep the smart pointer wrapping. This effectively
		 * creates a templated conversion operator to allow for implicit
		 * conversions between types and if they are incompatible then a
		 * compiler error will occur
		 * 
		 * @param ref The smart pointer to convert.
		 */
		template <typename newType>
		Ref (const Ref<newType>& ref)
		: mPointer (ref.getPointer()),
		  mCounter (ref.getPointer())
		{
			/* increase reference */
			if (mCounter) mCounter->increment();
		}
		
		
		/**
		 * Default constructor.
		 * A constructor with an empty reference.
		 */
		Ref () : mPointer(0), mCounter(0) {}
		
		
		
		/**
		 * Destructor.
		 * Upon deletion the reference will be released.
		 */
		virtual ~Ref ()
		{
			release ();
		}
		
		
		
		
		/* equality comparison operator */
		friend bool operator== (const Ref& lhs, const Ref& rhs) { return lhs.mPointer == rhs.mPointer; }
		friend bool operator== (const Ref& lhs, const T*   rhs) { return lhs.mPointer == rhs; }
		friend bool operator== (const T*   lhs, const Ref& rhs) { return lhs == rhs.mPointer; }
		
		/* inequality comparison operator */
		friend bool operator!= (const Ref& lhs, const Ref& rhs) { return !(lhs == rhs); }
		friend bool operator!= (const Ref& lhs, const T*   rhs) { return !(lhs == rhs); }
		friend bool operator!= (const T*   lhs, const Ref& rhs) { return !(lhs == rhs); }
		
		/* bool conversion operator */
		operator bool () { return mPointer != 0; }
		
		
		
		
		/**
		 * Copies the specified reference.
		 * When the reference is copied over its old reference is decremented 
		 * and released while the new reference is incremented and stored.
		 */
		Ref& operator= (const Ref& ref)
		{
			if (ref.mPointer != mPointer)
			{
				/* release current reference */
				release ();
				
				/* copy pointer and increase reference */
				mPointer = ref.mPointer;
				mCounter = ref.mCounter;
				if (mCounter) mCounter->increment();
			}
			
			return *this;
		}
		
		
		
		/* dereference operator */
		T& operator* () const { assert (mPointer); return *mPointer; }
		
		
		/* indirection operator */
		T* operator-> () const { return mPointer; }
		
		
		/**
		 * Retrieves the stored pointer being referenced.
		 */
		T* getPointer () const { return mPointer; }
		
		
		
		/**
		 * Releases the reference to the stored pointer.
		 * 
		 * Strong references not only decrement and nullify the reference but
		 * also free's the stored pointer if the reference counter reaches zero.
		 * Whilst the reference is automatically released upon destruction this
		 * method can be used to force its release.
		 */
		virtual void release ()
		{
			if (mPointer)
			{
				assert (mCounter->value() > 0);
				
				/* decrease reference */
				mCounter->decrement();
				
				/* free pointer */
				if (mCounter->value() == 0)
					/* use base destructor to allow incomplete types */
					delete mCounter;
				
				/* nullify reference */
				mPointer = 0;
				mCounter = 0;
			}
		}
		
		
		
	protected:
		/** The reference counter */
		ReferenceCounter* mCounter;
		
		/** The stored pointer */
		T* mPointer;
	};
	
	
	
	
	/**
	 * Weak intrusive smart pointer.
	 * 
	 * WeakRef maintains a weak reference to a pointer. A weak reference
	 * neither manages the reference counter nor any memory management. Its
	 * sole purpose is to provide compatibility with Ref when using raw
	 * pointers. It further allows the ability to distribute any reference
	 * without incrementing the counter and thus prolonging its life, although
	 * as a result the validity of a weak reference must always be checked
	 * with isValid() before it is used otherwise memory corruption may occur.
	 */
	template <class T>
	class COLLADA_PARSER_API WeakRef : public Ref<T>
	{
	public:
		/**
		 * Pointer constructor.
		 * 
		 * A constructor which stores the specified pointer. As a result the
		 * pointer must inherit from ReferenceCounter.
		 * 
		 * @param ptr The pointer to reference.
		 */
		explicit WeakRef (T* ptr) : Ref<T>(ptr) {}
		
		
		/**
		 * Copy constructor.
		 * @param ref The weak smart pointer to copy.
		 */
		WeakRef (const WeakRef& ref)
		{
			this->mPointer = ref.mPointer;
			this->mCounter = ref.mCounter;
		}
		
		
		/**
		 * Strong reference constructor.
		 * @param ref The strong smart pointer to copy.
		 */
		WeakRef (const Ref<T>& ref)
		{
			this->mPointer = ref.mPointer;
			this->mCounter = ref.mCounter;
		}
		
		
		/**
		 * Implicit conversion between smart pointers.
		 * @param ref The weak smart pointer to convert.
		 */
		template <typename newType>
		WeakRef (const WeakRef<newType>& ref)
		{
			this->mPointer = ref.getPointer();
			this->mCounter = ref.getPointer();
		}
		
		
		/**
		 * Implicit conversion between smart pointers.
		 * @param ref The strong smart pointer to convert.
		 */
		template <typename newType>
		WeakRef (const Ref<newType>& ref)
		{
			this->mPointer = ref.getPointer();
			this->mCounter = ref.getPointer();
		}
		
		
		/**
		 * Default constructor.
		 * A constructor with an empty reference.
		 */
		WeakRef () : Ref<T>() {}
		
		
		
		/**
		 * Releases the reference to the stored pointer.
		 * 
		 * Weak references simply nullify the reference without any memory
		 * management nor any counter management. Though WeakRef
		 * automatically releases the reference upon destruction this method
		 * can be used to force its release early.
		 */
		void release()
		{
			this->mPointer = 0;
			this->mCounter = 0;
		}
	};

}


#endif /* COLLADA_PARSER_SMART_POINTER_H_ */
