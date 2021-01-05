#pragma once
#include <vector>

namespace utils
{
	template <typename Info>
	class ISubject;

	template <typename Info>
	class IObserver
	{
		friend ISubject<Info>;

	public:
		virtual ~IObserver();

	protected:
		virtual void OnNotify(const Info& info) = 0;
	};

	template <typename Info>
	class ISubject
	{
	public:
		constexpr void Subscribe(IObserver<Info>& subscriber);
		constexpr void Unsubscribe(const IObserver<Info>& subscriber);
		constexpr void ClearSubscribers();

	protected:
		constexpr void Notify(const Info& info) const;

	private:
		std::vector<IObserver<Info>*> _subscribers{};
	};

	template <typename Info>
	constexpr void ISubject<Info>::Subscribe(IObserver<Info>& subscriber)
	{
		_subscribers.push_back(&subscriber);
	}

	template <typename Info>
	constexpr void ISubject<Info>::Unsubscribe(const IObserver<Info>& subscriber)
	{
		const auto&& iterator = std::remove(
			_subscribers.begin(), _subscribers.end(), subscriber);
		_subscribers.erase(iterator, _subscribers.end());
	}

	template <typename Info>
	constexpr void ISubject<Info>::ClearSubscribers()
	{
		_subscribers.clear();
	}

	template <typename Info>
	constexpr void ISubject<Info>::Notify(const Info& info) const
	{
		for (auto subscriber : _subscribers)
			subscriber->OnNotify(info);
	}

	template <typename Info>
	IObserver<Info>::~IObserver() = default;
}	 