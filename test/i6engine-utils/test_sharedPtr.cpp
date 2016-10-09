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

#include <array>
#include <thread>
#include <vector>

#include "i6engine/i6engineBuildSettings.h"

#include "i6engine/utils/weakPtr.h"

#include "gtest/gtest.h"

struct TestObject1 {
	static std::atomic<int> constructed;
	static std::atomic<int> deleted;
	int k;
	TestObject1() {
		constructed++;
	}
	~TestObject1() {
		deleted++;
	}
};
struct TestObject2 {
	static std::atomic<int> constructed;
	static std::atomic<int> deleted;
	TestObject2() {
		constructed++;
	}
	~TestObject2() {
		deleted++;
	}
};
struct TestObject3 {
	static std::atomic<int> constructed;
	static std::atomic<int> deleted;
	TestObject3() {
		constructed++;
	}
	virtual ~TestObject3() {
		deleted++;
	}
};
struct TestObject4 : public TestObject3 {
	TestObject4() {
	}
	~TestObject4() {
	}
};

// Polymorphism test
struct Base1 {
	static std::atomic<int> constructed;
	static std::atomic<int> deleted;
	Base1() {
		constructed++;
	}
	virtual ~Base1() {
		deleted++;
	}
};
struct Base2 {
	static std::atomic<int> constructed;
	static std::atomic<int> deleted;
	Base2() {
		constructed++;
	}
	virtual ~Base2() {
		deleted++;
	}
};
struct SubClassB1_1 : public Base1 {
	SubClassB1_1() : Base1() {
	}
	~SubClassB1_1() {
	}
};
struct SubClassB1_2 : public Base1 {
};
struct SubClassB12 : public Base1, public Base2 {
};
struct SubSubClass1 : public SubClassB1_1 {
};
struct SubSubClass2 : public SubClassB1_1 {
};

typedef i6e::utils::sharedPtr<TestObject1, TestObject1> Ptr1;
typedef i6e::utils::sharedPtr<TestObject2, TestObject2> Ptr2;
typedef i6e::utils::sharedPtr<TestObject3, TestObject3> Ptr3;
typedef i6e::utils::sharedPtr<TestObject4, TestObject3> Ptr4;
typedef i6e::utils::weakPtr<TestObject3> WeakPtr3;

std::atomic<int> TestObject1::constructed(0);
std::atomic<int> TestObject1::deleted(0);
std::atomic<int> TestObject2::constructed(0);
std::atomic<int> TestObject2::deleted(0);
std::atomic<int> TestObject3::constructed(0);
std::atomic<int> TestObject3::deleted(0);
std::atomic<int> Base1::constructed(0);
std::atomic<int> Base1::deleted(0);
std::atomic<int> Base2::constructed(0);
std::atomic<int> Base2::deleted(0);

struct OwningObject;
struct ParentObject;
struct ChildObject;
struct OtherChildObject;

typedef i6e::utils::sharedPtr<OwningObject, OwningObject> OwningPtr;
typedef i6e::utils::sharedPtr<ParentObject, ParentObject> ParentPtr;
typedef i6e::utils::weakPtr<ParentObject> WeakParentPtr;
typedef i6e::utils::sharedPtr<ChildObject, ParentObject> ChildPtr;
typedef i6e::utils::sharedPtr<OtherChildObject, ParentObject> OtherChildPtr;

struct OwningObject {
	static std::atomic<int> constructed;
	static std::atomic<int> deleted;
	OwningObject() : pList() {
		constructed++;
	}
	~OwningObject() {
		pList.clear();
		deleted++;
	}
	void add(const ParentPtr & pp) {
		pList.push_back(pp);
	}

private:
	std::vector<ParentPtr> pList;
};

static std::vector<WeakParentPtr> staticList;

void doAdd(const WeakParentPtr & wpp) {
	EXPECT_FALSE(wpp.expired());
	staticList.push_back(wpp);
}

struct ParentObject {
	static std::atomic<int> constructed;
	static std::atomic<int> deleted;
	ParentObject() {
		constructed++;
	}
	virtual ~ParentObject() {
		deleted++;
	}
	void setSelf(const WeakParentPtr & wpp) {
		_self = wpp;
	}

protected:
	WeakParentPtr _self;
};
struct ChildObject : public ParentObject {
	ChildObject() {
	}
	~ChildObject() {
	}
};
struct OtherChildObject : public ParentObject {
	OtherChildObject() {
	}
	~OtherChildObject() {
	}
	void init() {
		doAdd(_self);
	}
};

std::atomic<int> OwningObject::constructed(0);
std::atomic<int> OwningObject::deleted(0);
std::atomic<int> ParentObject::constructed(0);
std::atomic<int> ParentObject::deleted(0);

// This is a test fixture and NOT a sharedPtr!
class sharedPtr : public ::testing::Test {
public:
	void SetUp() {
		TestObject1::constructed = 0;
		TestObject1::deleted = 0;
		TestObject2::constructed = 0;
		TestObject2::deleted = 0;
		TestObject3::constructed = 0;
		TestObject3::deleted = 0;
		OwningObject::constructed = 0;
		OwningObject::deleted = 0;
		ParentObject::constructed = 0;
		ParentObject::deleted = 0;
		Base1::constructed = 0;
		Base1::deleted = 0;
		Base2::constructed = 0;
		Base2::deleted = 0;
	}

	void TearDown() {
		Ptr1::clear();
		Ptr2::clear();
		Ptr3::clear();
		i6e::utils::sharedPtr<Base1, Base1>::clear();
		i6e::utils::sharedPtr<Base2, Base2>::clear();
		OwningPtr::clear();
		ParentPtr::clear();
	}
};

TEST_F(sharedPtr, Constructor) {
	{
		EXPECT_EQ(0, TestObject1::constructed);
		Ptr1 p(new TestObject1());
		EXPECT_EQ(1, TestObject1::constructed);
	}
	Ptr1::clear();
	EXPECT_EQ(TestObject1::deleted, TestObject1::constructed);

	{
		EXPECT_EQ(0, TestObject2::constructed);
		Ptr2 p(new TestObject2());
		EXPECT_EQ(1, TestObject2::constructed);
	}
	Ptr2::clear();
	EXPECT_EQ(TestObject2::deleted, TestObject2::constructed);
}

Ptr1 func(Ptr1 p) {
	return Ptr1(p);
}

TEST_F(sharedPtr, Copying) {
	Ptr1 p(new TestObject1());
	for (int i = 0; i < 10; i++) {
		Ptr1 p2 = p;
		Ptr1 p3(p2);
	}
	EXPECT_EQ(1, TestObject1::constructed);
	EXPECT_EQ(0, TestObject1::deleted);
	Ptr1::clear();
}

TEST_F(sharedPtr, operators) {
	Ptr1 p1(new TestObject1());
	p1->k = 17;
	Ptr1 p2(new TestObject1());
	p2->k = 19;
	EXPECT_EQ(p1->k, 17);
	EXPECT_EQ(p2->k, 19);
}

TEST_F(sharedPtr, compare) {
	EXPECT_EQ(0, TestObject1::deleted);

	Ptr1 p1(new TestObject1());
	Ptr1 p2(new TestObject1());
	EXPECT_NE(p1, p2);

	p1 = p2;

	EXPECT_EQ(p1, p2);

	EXPECT_EQ(0, TestObject1::deleted);

	Ptr1::clear();
	EXPECT_EQ(1, TestObject1::deleted);

	p1 = Ptr1(nullptr);
	Ptr1::clear();
	EXPECT_EQ(1, TestObject1::deleted);

	p2 = p1;
	Ptr1::clear();
	EXPECT_EQ(2, TestObject1::deleted);
}

TEST_F(sharedPtr, nullPtr) {
	Ptr1 p(nullptr);
	Ptr1 p2(p);
	Ptr1 p3;
	EXPECT_EQ(p, nullptr);
	EXPECT_EQ(p2, nullptr);
	EXPECT_EQ(p, p2);
	EXPECT_EQ(p, p3);
	TestObject1 * a = new TestObject1();
	EXPECT_NE(p, a);
	EXPECT_NE(p2, a);
	EXPECT_NE(p3, a);
	p = p2 = p3 = Ptr1(a);
	EXPECT_EQ(p, a);
	EXPECT_EQ(p2, a);
	EXPECT_EQ(p3, a);
}

TEST_F(sharedPtr, get) {
	TestObject1 * t1 = new TestObject1();
	TestObject1 * t2 = new TestObject1();
	Ptr1 p(t1);
	Ptr1 p2(t2);

	EXPECT_EQ(t1, p.get());
	EXPECT_EQ(t2, p2.get());

	p = p2;

	EXPECT_EQ(t2, p.get());
}

void spawn(Ptr1 p) {
	std::vector<Ptr1> mass(1000, p);
	for(int i = 0; i < 10000; i++) {
		Ptr1 p2(p);
		Ptr1::clear();
	}
}

TEST_F(sharedPtr, multiThread) {
	std::vector<std::thread *> v;

	{
		Ptr1 p = Ptr1(new TestObject1());
		for(int i = 0; i < 10; i++) {
			v.push_back(new std::thread(std::bind(spawn, p)));
		}
		for (size_t i = 0; i < 10; i++) {
			Ptr1::clear();
			v[i]->join();
			delete v[i];
		}
		EXPECT_EQ(0, TestObject1::deleted);
		Ptr1::clear();
	}
	EXPECT_EQ(0, TestObject1::deleted);
	Ptr1::clear();
	EXPECT_EQ(1, TestObject1::deleted);
	EXPECT_EQ(1, TestObject1::constructed);
}

TEST_F(sharedPtr, derived) {
	{
		Ptr3 p1 = Ptr3(new TestObject3());
	}

	EXPECT_EQ(1, TestObject3::constructed);
	EXPECT_EQ(0, TestObject3::deleted);

	Ptr3::clear();

	EXPECT_EQ(1, TestObject3::constructed);
	EXPECT_EQ(1, TestObject3::deleted);

	{
		Ptr4 p2 = Ptr4(new TestObject4());
		Ptr3 p3 = Ptr4(new TestObject4()); 
	}

	EXPECT_EQ(3, TestObject3::constructed);
	EXPECT_EQ(1, TestObject3::deleted);

	Ptr3::clear();

	EXPECT_EQ(3, TestObject3::constructed);
	EXPECT_EQ(3, TestObject3::deleted);

	{
		Ptr3 p4 = Ptr4(new TestObject4());
		Ptr4 p5 = i6e::utils::dynamic_pointer_cast<TestObject4>(p4);
	}

	EXPECT_EQ(4, TestObject3::constructed);
	EXPECT_EQ(3, TestObject3::deleted);

	Ptr3::clear();

	EXPECT_EQ(4, TestObject3::constructed);
	EXPECT_EQ(4, TestObject3::deleted);
}

TEST_F(sharedPtr, make_shared) {
	{
		Ptr3 p1 = i6e::utils::make_shared<TestObject3, TestObject3>();
	}

	EXPECT_EQ(1, TestObject3::constructed);
	EXPECT_EQ(0, TestObject3::deleted);

	Ptr3::clear();

	EXPECT_EQ(1, TestObject3::constructed);
	EXPECT_EQ(1, TestObject3::deleted);

	{
		Ptr4 p2 = i6e::utils::make_shared<TestObject4, TestObject3>();
	}

	EXPECT_EQ(2, TestObject3::constructed);
	EXPECT_EQ(1, TestObject3::deleted);

	Ptr3::clear();

	EXPECT_EQ(2, TestObject3::constructed);
	EXPECT_EQ(2, TestObject3::deleted);

	{
		Ptr3 p3 = i6e::utils::make_shared<TestObject4, TestObject3>();
		Ptr4 p4 = i6e::utils::dynamic_pointer_cast<TestObject4>(p3);
	}

	EXPECT_EQ(3, TestObject3::constructed);
	EXPECT_EQ(2, TestObject3::deleted);

	Ptr3::clear();

	EXPECT_EQ(3, TestObject3::constructed);
	EXPECT_EQ(3, TestObject3::deleted);
}

TEST_F(sharedPtr, weakPtr) {
	{
		Ptr3 p1 = i6e::utils::make_shared<TestObject3, TestObject3>();
		WeakPtr3 wp1 = p1;

		EXPECT_FALSE(wp1.expired());
	}

	EXPECT_EQ(1, TestObject3::constructed);
	EXPECT_EQ(0, TestObject3::deleted);

	Ptr3::clear();

	EXPECT_EQ(1, TestObject3::constructed);
	EXPECT_EQ(1, TestObject3::deleted);

	WeakPtr3 wp2;

	{
		Ptr3 p2 = i6e::utils::make_shared<TestObject3, TestObject3>();
		wp2 = p2;

		EXPECT_FALSE(wp2.expired());
	}

	EXPECT_TRUE(wp2.expired());

	EXPECT_EQ(nullptr, wp2.get());

	EXPECT_EQ(2, TestObject3::constructed);
	EXPECT_EQ(1, TestObject3::deleted);

	Ptr3::clear();

	EXPECT_EQ(2, TestObject3::constructed);
	EXPECT_EQ(2, TestObject3::deleted);

	{
		Ptr3 p3 = i6e::utils::make_shared<TestObject4, TestObject3>();
		Ptr4 p4 = i6e::utils::dynamic_pointer_cast<TestObject4>(p3);
		WeakPtr3 wp3 = p4;

		EXPECT_FALSE(wp3.expired());

		EXPECT_EQ(p3, wp3.get());
	}

	EXPECT_EQ(3, TestObject3::constructed);
	EXPECT_EQ(2, TestObject3::deleted);

	Ptr3::clear();

	EXPECT_EQ(3, TestObject3::constructed);
	EXPECT_EQ(3, TestObject3::deleted);
}

TEST_F(sharedPtr, usageExample) {
	EXPECT_EQ(0, OwningObject::constructed);
	EXPECT_EQ(0, OwningObject::deleted);
	EXPECT_EQ(0, ParentObject::constructed);
	EXPECT_EQ(0, ParentObject::deleted);

	{
		OwningPtr op = i6e::utils::make_shared<OwningObject, OwningObject>();

		EXPECT_EQ(1, OwningObject::constructed);
		EXPECT_EQ(0, OwningObject::deleted);

		{
			ParentPtr pp = i6e::utils::make_shared<ParentObject, ParentObject>();
			op->add(pp);
			pp->setSelf(pp);

			ParentPtr::clear();

			EXPECT_EQ(1, ParentObject::constructed);
			EXPECT_EQ(0, ParentObject::deleted);

			ParentPtr pp2 = i6e::utils::make_shared<ChildObject, ParentObject>();
			op->add(pp2);
			pp2->setSelf(pp2);

			ParentPtr::clear();

			EXPECT_EQ(2, ParentObject::constructed);
			EXPECT_EQ(0, ParentObject::deleted);

			{
				ChildPtr pp3 = i6e::utils::make_shared<ChildObject, ParentObject>();
				op->add(pp3);
				pp3->setSelf(pp3);

				ParentPtr::clear();

				EXPECT_EQ(3, ParentObject::constructed);
				EXPECT_EQ(0, ParentObject::deleted);

			}

			ParentPtr::clear();

			EXPECT_EQ(3, ParentObject::constructed);
			EXPECT_EQ(0, ParentObject::deleted);

			{
				OtherChildPtr pp4 = i6e::utils::make_shared<OtherChildObject, ParentObject>();
				op->add(pp4);
				pp4->setSelf(pp4);

				pp4->init();

				ParentPtr::clear();

				EXPECT_EQ(4, ParentObject::constructed);
				EXPECT_EQ(0, ParentObject::deleted);

			}
		}

		ParentPtr::clear();

		EXPECT_EQ(4, ParentObject::constructed);
		EXPECT_EQ(0, ParentObject::deleted);
	}

	ParentPtr::clear();

	EXPECT_EQ(4, ParentObject::constructed);
	EXPECT_EQ(0, ParentObject::deleted);

	OwningPtr::clear();

	ParentPtr::clear();

	EXPECT_EQ(1, OwningObject::constructed);
	EXPECT_EQ(1, OwningObject::deleted);
	EXPECT_EQ(4, ParentObject::constructed);
	EXPECT_EQ(4, ParentObject::deleted);
}

TEST_F(sharedPtr, selfAssignment) {
	EXPECT_EQ(0, OwningObject::constructed);
	EXPECT_EQ(0, OwningObject::deleted);
	EXPECT_EQ(0, ParentObject::constructed);
	EXPECT_EQ(0, ParentObject::deleted);

	{
		OwningPtr op1 = i6e::utils::make_shared<OwningObject, OwningObject>();

		EXPECT_EQ(1, OwningObject::constructed);
		EXPECT_EQ(0, OwningObject::deleted);

		op1 = op1;

		EXPECT_EQ(1, OwningObject::constructed);
		EXPECT_EQ(0, OwningObject::deleted);
	}

	OwningPtr::clear();

	EXPECT_EQ(1, OwningObject::constructed);
	EXPECT_EQ(1, OwningObject::deleted);
}

TEST_F(sharedPtr, polymorphismGoodSTL) {
	// INFO: this is just a sample of polymorphismGood using std::shared_ptr
	// multiple baseclasses
	Base1::constructed = 0;
	Base1::deleted = 0;
	Base2::constructed = 0;
	Base2::deleted = 0;
	{
		SubClassB12 * bPtr = new SubClassB12();
		std::shared_ptr<SubClassB12> b3; // should be deleted after b2
		{
			std::shared_ptr<SubClassB12> b1(bPtr);
			std::shared_ptr<Base1> b2(b1);
			b3 = std::dynamic_pointer_cast<SubClassB12>(b2);
			std::shared_ptr<Base2> b4 = b3;

			// Base1 and Base2 constructed
			ASSERT_EQ(1, Base1::constructed);
			ASSERT_EQ(0, Base1::deleted);
			ASSERT_EQ(1, Base2::constructed);
			ASSERT_EQ(0, Base2::deleted);
		}
		// nothing changed
		ASSERT_EQ(1, Base1::constructed);
		ASSERT_EQ(0, Base1::deleted);
		ASSERT_EQ(1, Base2::constructed);
		ASSERT_EQ(0, Base2::deleted);
	}
	// now all deleted
	ASSERT_EQ(1, Base1::constructed);
	ASSERT_EQ(1, Base1::deleted);
	ASSERT_EQ(1, Base2::constructed);
	ASSERT_EQ(1, Base2::deleted);
}

TEST_F(sharedPtr, polymorphismGood) {
	Base1::constructed = 0;
	Base1::deleted = 0;
	Base2::constructed = 0;
	Base2::deleted = 0;
	using i6e::utils::sharedPtr;

	{
		SubSubClass1 * aPtr = new SubSubClass1();
		sharedPtr<SubSubClass1, Base1> a1(aPtr);
		sharedPtr<Base1, Base1> a2 = a1;
		sharedPtr<SubSubClass1, Base1> a3 = i6e::utils::dynamic_pointer_cast<SubSubClass1, Base1>(a2);
		EXPECT_EQ(aPtr, a1.get());
		EXPECT_EQ(aPtr, a2.get());
		EXPECT_EQ(aPtr, a3.get());
	}
	sharedPtr<Base1, Base1>::clear();
	ASSERT_EQ(1, Base1::constructed);
	ASSERT_EQ(1, Base1::deleted);
	ASSERT_EQ(0, Base2::constructed);
	ASSERT_EQ(0, Base2::deleted);

	// multiple baseclasses
	Base1::constructed = 0;
	Base1::deleted = 0;
	Base2::constructed = 0;
	Base2::deleted = 0;
	{
		SubClassB12 * bPtr = new SubClassB12();
		sharedPtr<SubClassB12, Base1> b3; // should be deleted after b2
		{
			sharedPtr<SubClassB12, Base1> b1(bPtr);
			sharedPtr<Base1, Base1> b2(b1);
			b3 = i6e::utils::dynamic_pointer_cast<SubClassB12, Base1>(b2);
			sharedPtr<Base2, Base1> b4 = b3;

			// Base1 and Base2 constructed
			ASSERT_EQ(1, Base1::constructed);
			ASSERT_EQ(0, Base1::deleted);
			ASSERT_EQ(1, Base2::constructed);
			ASSERT_EQ(0, Base2::deleted);
		}
		// nothing changed
		ASSERT_EQ(1, Base1::constructed);
		ASSERT_EQ(0, Base1::deleted);
		ASSERT_EQ(1, Base2::constructed);
		ASSERT_EQ(0, Base2::deleted);
	}
	sharedPtr<Base1, Base1>::clear();
	// now all deleted
	ASSERT_EQ(1, Base1::constructed);
	ASSERT_EQ(1, Base1::deleted);
	ASSERT_EQ(1, Base2::constructed);
	ASSERT_EQ(1, Base2::deleted);
}

TEST_F(sharedPtr, polymorphismFoo) {
	using i6e::utils::sharedPtr;

	// invalid cast. This should NOT even compile
//	sharedPtr<Base1, Base1> a1(new Base1());
//	sharedPtr<Base2, Base2> a2 = a1;

	// invalid dynamic cast shall return in nulltr
	SubClassB1_1 * bPtr = new SubClassB1_1();
	sharedPtr<SubClassB1_1, Base1> b1(bPtr);
	sharedPtr<Base1, Base1> b2 = b1;
	sharedPtr<SubClassB1_2, Base1> b3 = i6e::utils::dynamic_pointer_cast<SubClassB1_2, Base1>(b2);
	EXPECT_EQ(bPtr, b1.get());
	EXPECT_EQ(bPtr, b2.get());
	EXPECT_EQ(nullptr, b3.get());

	SubSubClass1 * cPtr = new SubSubClass1();
	sharedPtr<SubSubClass1, Base1> c1(cPtr);
	sharedPtr<Base1, Base1> c2 = c1;
	sharedPtr<SubSubClass2, Base1> c3 = i6e::utils::dynamic_pointer_cast<SubSubClass2, Base1>(c2);
	EXPECT_EQ(cPtr, c1.get());
	EXPECT_EQ(cPtr, c2.get());
	EXPECT_EQ(nullptr, c3.get());
}

// This test only deals with nullptrs
// it won't expect anything but can detect segfaults, memory leaks etc.
TEST_F(sharedPtr, nullPtrFoo) {
	using i6e::utils::sharedPtr;

	sharedPtr<SubClassB12, Base1> p1;
	sharedPtr<SubClassB12, Base1> p2 = p1;
	sharedPtr<SubClassB12, Base1> p3(p2);
	sharedPtr<SubClassB12, Base1> p4;
	p4 = p3;
}

TEST_F(sharedPtr, nullPtrFoo2) {
	using i6e::utils::sharedPtr;

	sharedPtr<Base1, Base1> p1(new SubClassB1_1());
	sharedPtr<SubClassB1_1, Base1> p2 = i6e::utils::dynamic_pointer_cast<SubClassB1_1>(p1);
	sharedPtr<SubClassB1_1, Base1> p3(p2);
	sharedPtr<SubClassB1_1, Base1> p4;
	p4 = p3;
}

TEST_F(sharedPtr, replacement) {
	using i6e::utils::sharedPtr;
	using i6e::utils::make_shared;
	std::array<sharedPtr<Base1, Base1>, 1> arr;
	auto sp = make_shared<Base1, Base1>();
	arr[0] = sp;
}

// removed this test, check whether it makes sense... accessing the same entry of a vector reading and writing is never threadsafe!
/*TEST_F(sharedPtr, weakPtrRaceCondition) {
	using i6e::utils::sharedPtr;
	using i6e::utils::weakPtr;
	using i6e::utils::make_shared;
	const size_t RUNS = 1000;
	std::array<sharedPtr<Base1, Base1>, 100> arr;
	for (size_t i = 0; i < arr.size(); i++) {
		arr[i] = make_shared<Base1, Base1>();
	}
	std::thread thrdPurge([&arr, RUNS]() {
		for (size_t i = 0; i < arr.size(); i++) {
			arr[i] = make_shared<Base1, Base1>();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			arr[i] = make_shared<Base1, Base1>();
		}
	});
	std::thread thrdCopy([&arr, RUNS]() {
		for (size_t i = 0; i < RUNS; i++) {
			for (size_t j = 0; j < arr.size(); j++) {
				weakPtr<Base1> wp = arr[j];
				EXPECT_NE(wp, nullptr);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	});
	for (size_t i = 0; i < RUNS; i++) {
		sharedPtr<Base1, Base1>::clear();
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
	thrdPurge.join();
	thrdCopy.join();
}*/
