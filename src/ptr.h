#ifndef ARUAB_PTR_H__
#define ARUAB_PTR_H__
#pragma once

#include <list>
#include <memory>

namespace arua {

template <typename T>
class Ptr {
	template <typename U> friend class Ptr;
public:
	Ptr()
			: instances(new std::list<Ptr<T> *>()) {
		this->attach();
	}

	Ptr(const Ptr<T> &other)
			: instances(other.instances)
			, ptr(other.ptr) {
		this->attach();
	}

	Ptr(T *ptr)
			: instances(new std::list<Ptr<T> *>())
			, ptr(ptr) {
		this->attach();
	}

	~Ptr() {
		this->detach();
	}

	void reset(T *t) {
		this->detach();
		this->instances.reset(new std::list<Ptr<T> *>());
		this->attach();
		this->ptr.reset(t);
	}

	void replace(T *t) {
		std::shared_ptr<T> newPtr(t);

		for (Ptr<T> * &inst : *this->instances) {
			inst->ptr = newPtr;
		}
	}

	void replace(Ptr<T> other) {
		// this is really a merge...

		// gets destructed after this method
		auto insts = this->instances;

		for (Ptr<T> * &inst : *insts) {
			// set the value
			inst->ptr = other.ptr;

			// replace the instance list to merge
			inst->instances = other.instances;

			// attach to new instance list
			inst->attach();
		}
	}

	T * get() {
		return this->ptr.get();
	}

	const T * get() const {
		return this->ptr.get();
	}

	T & operator *() {
		return *this->ptr;
	}

	const T & operator *() const {
		return *this->ptr;
	}

	T * operator ->() {
		return this->ptr.operator ->();
	}

	const T * operator ->() const {
		return this->ptr.operator ->();
	}

	bool operator==(const Ptr<T> &other) const {
		return this->ptr == other.ptr;
	}

	void operator=(const Ptr<T> &other) {
		this->detach();
		this->instances = other.instances;
		this->attach();
		this->ptr = other.ptr;
	}

	void swap(Ptr<T> &other) {
		this->detach();
		other.detach();
		this->instance.swap(other.instances);
		this->ptr.swap(other.ptr);
		this->attach();
		other.attach();
	}

	template <typename... Args>
	static Ptr<T> make(Args... args) {
		return Ptr<T>(new T(args...));
	}

	template <typename U>
	Ptr<U> as() {
		Ptr<U> u;
		u.ptr = std::static_pointer_cast<U>(this->ptr);
		u.instances = ((Ptr<U> *)this)->instances;
		u.attach();
		return u;
	}

	operator bool() const throw() {
		return bool(this->ptr);
	}

private:
	void detach() {
		if (this->instances) {
			this->instances->erase(this->thisIterator);
		}
	}

	void attach() {
		if (this->instances) {
			this->thisIterator = this->instances->insert(this->instances->end(), this);
		}
	}

	std::shared_ptr<std::list<Ptr<T> *>> instances;
	typename std::list<Ptr<T> *>::iterator thisIterator;
	std::shared_ptr<T> ptr;
};

}

#endif
