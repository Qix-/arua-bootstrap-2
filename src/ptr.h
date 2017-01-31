#ifndef ARUAB_PTR_H__
#define ARUAB_PTR_H__
#pragma once

#include <list>
#include <memory>

namespace arua {

template <typename T>
class Ptr {
public:
	Ptr()
			: instances(new std::list<Ptr<T> *>()) {
		this->attach();
	}

	Ptr(const Ptr<T> &other)
			: instances(other.instances) {
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
