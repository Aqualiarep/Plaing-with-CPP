#pragma once
#include <assert.h>

namespace {
	template<unsigned int ... Numbers>
	struct indexes {};

	template <unsigned int N, unsigned int ... Numbers>
	struct index_gen : index_gen<N - 1, N - 1, Numbers ...> {};

	template <unsigned int N>
	struct index_gen < N > : index_gen<N - 1, N - 1>{};

	template <unsigned int ... Numbers>
	struct index_gen < 0, Numbers ...> : indexes<Numbers ...>{};
}

template <class ... Args> struct DelayInvoke;

template <class R, class ... Args>
struct DelayInvoke<R, Args...> {
	typedef std::function<DelayInvoke<R, Args ...>(Args ...)> Ft;

	Ft func;
	std::tuple<Args ...> args;
	R result;
	bool stoped = false;
	bool stub = false;

	DelayInvoke(Ft f, Args ... _args) : func(f), args(_args ...) {}
	DelayInvoke(R _result) : result(_result), stoped(true) {}
	DelayInvoke() : stub(true) {}

	operator R () const { assert(!stub); return result; }
	operator bool () const { assert(!stub); return !stoped; }

	template<unsigned int ... Numbers>
	DelayInvoke<R, Args...> CallFunc(std::tuple<Args...> tuple, indexes<Numbers...>)
	{
		return func(std::get<Numbers>(tuple)...);
	}

	DelayInvoke<R, Args...> CallFunc(std::tuple<Args ...> tuple)
	{
		return CallFunc(tuple, index_gen<sizeof...(Args)>());
	}

	DelayInvoke<R, Args...> operator() () {
		assert(!stub);
		DelayInvoke<R, Args...> delay = CallFunc(args);
		return delay;
	}
};

template <class R, class ... Args>
R TailCall(DelayInvoke<R, Args ...> invoke_)
{
	DelayInvoke<R, Args...> result;
	while (invoke_) {
		std::swap(invoke_, invoke_());
	}
	return invoke_;
}

DelayInvoke<uint64_t, uint64_t, uint64_t> calc_calls_count_trr(uint64_t required, uint64_t accum = 0) {
	auto me = &calc_calls_count_trr;
	if (required == 1) {
		return ++accum;
	}
	return DelayInvoke<uint64_t, uint64_t, uint64_t>(me, --required, ++accum);
}

uint64_t calc_calls_count_simple(uint64_t required, uint64_t accum = 0) {
	auto me = &calc_calls_count_trr;
	if (required == 1) {
		return ++accum;
	}
	return calc_calls_count_simple(--required, ++accum);
}